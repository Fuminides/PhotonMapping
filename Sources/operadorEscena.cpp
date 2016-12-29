#include "operadorEscena.hpp"

/*
 * Añade una esfera al array de figuras de la escena
 */
void operadorEscena::anyadirFigura(Figura * f){
    if (f->isLuz()){
        std::vector<Luz> luces;
        luces = f->getLuces();
        for ( Luz luz: luces){
            anyadirLuz(luz);
        }
    }
    figuras.push_back(f);
}

/**
 * Traza rayos por la pantalla y detecta
 * que figuras han interceptado.
 */
void operadorEscena::dibujar(){
    Color defecto;
    int distancia, min = -1, filas = 0;
    Figura * choque;

    defecto.set_values(0,0,0, NORMALIZAR_COLORES);
    std::list<Rayo> rayos = camara.trazarRayos();
    
    std::vector<Color> pixels;
    pixels.reserve(rayos.size());
    
    int i = 0;
    float progress = 0.0;
    unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    if ( concurentThreadsSupported == 0) concurentThreadsSupported = 1;
    cout << "Numero de cores en uso: " << concurentThreadsSupported << "\n";
    int threads_usados = 0;
    int pixelsPosition[50]; //Si tiene mas de 50 cores no va, pero bueno, si tienes ese equipo no estaras usando este trazador.

    std::vector<std::future<Color>> futures;
    futures.reserve(concurentThreadsSupported);
    for ( Rayo rayo : rayos){
        min = interseccion(rayo, &choque);

        if ( min != -1){
            if ( !choque->isLuz() ) {
                Punto puntoRender, origenRayos = camara.getPosicion();
                Vector direccion = rayo.getVector();
                puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                    origenRayos.getZ() + direccion.getZ() * min);
                futures[threads_usados] = (async(&operadorEscena::renderizar, this, puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), REFRACCION_MEDIO, true, PATH_LEN));
                pixelsPosition[threads_usados] = i; //Nos guardamos el numero de pixel del n thread.
                threads_usados = (threads_usados + 1)%concurentThreadsSupported; 

                if (threads_usados == 0) //Hemos agotado los cores
                {
                    for (int cuenta = 0; cuenta < concurentThreadsSupported; ++cuenta)
                    {
                        pixels[pixelsPosition[cuenta]] = futures[cuenta].get();
                    }
                   
                }

                min = -1;
            }
            else{
                Color colorAux = choque->getColor();
                pixels[i] = (colorAux);
                min = -1;
            }
        }
        else{
                pixels[i] = (defecto);
        }

        i++;
        progress= i*1.0 / (rayos.size()*1.0);

        if ((progress < 1.0) && (i % 500 == 0)) {
            int barWidth = 70;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " " <<  std::to_string(i) << "/" << rayos.size() << " \r";
            std::cout.flush();
        }
        //std::cout << std::endl;
    }
            
    for (int cuenta = 0; cuenta < concurentThreadsSupported; ++cuenta)
    {
        if ( !pixels[pixelsPosition[cuenta]].iniciado() ) pixels[pixelsPosition[cuenta]] = futures[cuenta].get();
    }
                   

    double areaPixel = (camara.getResX() * camara.getResY()) *1.0/ (rayos.size()*1.0);
    int fila = camara.getResX() / sqrt(areaPixel),
        columna = camara.getResY() / sqrt(areaPixel),
        nPixels = rayos.size();
    
    std::ofstream myfile;
    myfile.open ("example.ppm");
    myfile << "P6 " << fila << " " << columna << " 255\n";

    if ( NORMALIZAR_COLORES ){
        double max = 0;
        for ( Color color : pixels){
            if ( color.max() > max) max = color.max();
        }
        if ( max > 255){
            for ( Color color : pixels){
                color.multiplicar(1.0/max);
                color.multiplicar(255);
            }    
        }
    }

    for ( int i = 0; i <rayos.size(); ++i){
        myfile << pixels[i].splashR();
        myfile << pixels[i].splashG();
        myfile << pixels[i].splashB();
    }

    myfile.close();
}

void operadorEscena::setCamara(Camara c){
    camara = c;
}

void operadorEscena::anyadirLuz(Luz l){
    luces.push_back(l);
}

Color operadorEscena::renderizar(Punto p, Figura * figura, int numeroRebotes, Punto origenVista, double refraccionMedio, bool indirecto, int seguirCamino){
    double distancia; bool libre, libreCompleto = true;
    Color negro;
    negro.set_values(0,0,0, NORMALIZAR_COLORES);
    Color inicial = negro;
    double kd = AMBIENTE; 
    Vector dirLuz;
    double bdrf;
    int min;

    for ( Luz luz: luces){
        libre = true;
        Vector dirLuz = restaPuntos(luz.getOrigen(), p);
        Rayo puntoDirLuz;
        double dLuz = dirLuz.modulo();
        dirLuz.normalizar();
        puntoDirLuz.set_values(p, dirLuz);

        min = -1;

        for (int i = 0; i < figuras.size(); i++){
            distancia = figuras[i]->intersectar(puntoDirLuz);

            if ( distancia >= 0 ){
                if ((!figuras[i]->isLuz()) && (distancia < dLuz)){ //Las luces de area no intersectan
                    min = distancia;
                    break;
                }
            }
        }

        if ( min > -1 ){
            libre = false;
        }

        if ( libre ){ //Si no hemos encontrado nada que tape la luz
            Color auxC = figura->getColor(); 
            auxC.multiplicar(AMBIENTE/M_PI); //Mismo termino difuso para ambas BRDF.
            luz.atenuar(restaPuntos(p, luz.getOrigen()).modulo());

            if ( luz.getColor().max() > 0.0001){ //Si no le llega la luz con un minimo de intensidad es como si no le llegara

                if ( figura->getBRDF() == 0){
                    bdrf = phong(figura, p, dirLuz,restaPuntos(origenVista,p));   
                } 
                else if (figura->getBRDF() == 1){
                    bdrf = ward(restaPuntos(origenVista,p), dirLuz, figura->normal(p), p);
                }

                Color auxColor = luz.getColor();
                auxColor.multiplicar(bdrf);

                //Sumamos el especular y el difuso (no hay ambiente)
                inicial.sumar(auxColor);
                inicial.sumar(auxC);
                Vector R = restaVectores(valorPorVector(figura->normal(p), 2 * productoEscalar(dirLuz, figura->normal(p))), dirLuz); //Ya normalizado
                double prodAux = productoEscalar(figura->normal(p), R );
                if ( prodAux < 0 ) prodAux = -prodAux;
                inicial.multiplicar(prodAux); //Ya normalizado

                //Caminos especulares
                if ( numeroRebotes > 0){
                    Vector R, dirLuz2 = restaPuntos(p, origenVista);
                    dirLuz2.normalizar();

                    if ( figura->getReflejo() > 0.0){ //No calculamos si no hay reflejo
                        Vector nAux = figura->normal(p);

                        if ( productoEscalar(restaPuntos(origenVista,p), nAux) < 0 ) nAux = valorPorVector(nAux, -1);

                        R = restaVectores(dirLuz2, valorPorVector(valorPorVector(nAux, productoEscalar(dirLuz2,nAux)), 2));
                        R.normalizar();
                        Color auxC = reboteEspecular(figura, p, R, numeroRebotes);
                        auxC.multiplicar(figura->getReflejo());
                        inicial.sumar(auxC);
                    }

                    if( figura->getCoefRefraccion() > 0.0){ //No calculamos si no hay refraccion
                        auxC = refraccionEspecular(figura, p, restaPuntos(p, origenVista), refraccionMedio, figura->getRefraccion(), numeroRebotes);
                        auxC.multiplicar (figura->getCoefRefraccion());
                        inicial.sumar(auxC);
                    }
                }

                inicial.multiplicar(K_LUZ_DIR); //Solo para debug. (El k es 1)
            }
        }
    }

    min = -1;

    if ( indirecto ){ //Si hay que calcular luz indirecta
        Figura * choque;
        Punto auxP;
        auxP.set_values(0,0,0);
        Montecarlo montecarlo;

        if ( PATH_TRACING && (seguirCamino < PATH_LEN) ) montecarlo.set_values(restaPuntos(p,auxP), figura->normal(p), 1); //Si estamos en un camino de path tracing, solo queremos un rayo.
        else montecarlo.set_values(restaPuntos(p,auxP), figura->normal(p), NUMERO_RAYOS_INDIRECTA); //Si estamos en la primera iteracion de path tracing o no esta activado, lanzamos todos.
        
        list<Vector> vecIndir = montecarlo.calcularw();
        double exitos = 0.0;
        if (!PATH_TRACING){ //Si no usamos path tracing, generamos y lanzamos los rayos y ya esta.
            Color cIndir;
            cIndir.set_values(0,0,0, NORMALIZAR_COLORES);
            double rojo = 0.0, verde = 0.0, azul = 0.0;
            montecarlo.set_values(restaPuntos(p,auxP), figura->normal(p), 1);

            //Igual que para el reflejo y refraccion
            //Hacemos la media de todos los puntos renderizados con montecarlo
            for ( Vector vec : vecIndir){
                Rayo rayo;
                rayo.set_values(p, vec);
                min = interseccion(rayo, &choque);

                if ( min != -1){
                    Punto puntoRender, origenRayos = camara.getPosicion();
                    Vector direccion = rayo.getVector();
                    puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                        origenRayos.getZ() + direccion.getZ() * min);

                    Color cIndir = renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), refraccionMedio, false, 0);
                    
                    if (cIndir.max() > 0.0) {
                        Color auxC = figura->getColor(); 
                        auxC.multiplicar(AMBIENTE/M_PI); //Mismo termino difuso para ambas BRDF.

                        exitos += 1.0;
                        Vector vAux = restaPuntos(puntoRender,p);
                        vAux.normalizar();


                        if ( figura->getBRDF() == 0){
                            bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p));   
                        } 
                        else if (figura->getBRDF() == 1){
                            bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p);
                        }
                        
                        cIndir.multiplicar(bdrf);
                        //cIndir.sumar(auxC);

                        //Vamos acumulando los valores para hacer la media
                        rojo += cIndir.splashR();
                        verde += cIndir.splashG();
                        azul += cIndir.splashB();

                        min = -1;
                    }
                }
            }

            rojo = rojo / (vecIndir.size()*1.0);
            verde = verde / (vecIndir.size()*1.0);
            azul = azul/ (vecIndir.size()*1.0);

            cIndir.set_values(rojo, verde, azul, NORMALIZAR_COLORES);
            inicial.sumar(cIndir);
        }
        else{
            if ( seguirCamino == 0) { return inicial; }//Si hemos llegado al final del camino, devolvemos la directa.

            //Igual que si no hubiese path tracing (salvando ligeros cambios)
            Color cIndir;
            cIndir.set_values(0,0,0, NORMALIZAR_COLORES);
            double rojo = 0, verde = 0, azul = 0;
            double exitos = 1.0;
            montecarlo.set_values(restaPuntos(p,auxP), figura->normal(p), 1);

            for (Vector vec : vecIndir){
                Figura * figuraAux;
                Rayo rayo;
                rayo.set_values(p, vec);
                min = interseccion(rayo, &figuraAux);
                int i;

                //Si no choca con nada, podemos intentar hacia otro lado
                for ( i = 0; (min == -1) && i <2; i++){
                    Vector vect = montecarlo.calcularw().front();
                    rayo.set_values(p, vect);
                    min = interseccion(rayo, &figuraAux);
                }

                if ( min != -1){
                    Punto puntoRender, origenRayos = p;
                    Vector direccion = rayo.getVector();

                    puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                        origenRayos.getZ() + direccion.getZ() * min);
                    Color colorAux;

                    if ( !figuraAux->isLuz() ){
                        colorAux = renderizar(puntoRender, figuraAux, NUMERO_REBOTES, camara.getPosicion(), refraccionMedio, true, seguirCamino - 1);

                        if ( colorAux.max() > 0.0) {
                            exitos += 1.0;

                            Vector vAux = restaPuntos(puntoRender,p);
                            double distance = vAux.modulo();
                            vAux.normalizar();

                            if ( figura->getBRDF() == 0){
                                bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p));   
                            } 
                            else if (figura->getBRDF() == 1){
                                bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p);
                            }
                            
                            colorAux.multiplicar(bdrf);

                            rojo += colorAux.splashR();
                            verde += colorAux.splashG();
                            azul += colorAux.splashB();
                        }
                    }
                    else {
                        Luz lAux = figuraAux->getLuces()[0];
                        lAux.atenuar(min);
                        colorAux = lAux.getColor(); 
                        exitos += 1.0;

                        Vector vAux = restaPuntos(puntoRender,p);
                        double distance = vAux.modulo();
                        vAux.normalizar();

                        if ( figura->getBRDF() == 0){
                            bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p));   
                        } 
                        else if (figura->getBRDF() == 1){
                            bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p);
                        }
                        
                        colorAux.multiplicar(bdrf);

                        rojo += colorAux.splashR();
                        verde += colorAux.splashG();
                        azul += colorAux.splashB();
                        }
                }
            }

            if ( seguirCamino == PATH_LEN)  rojo = rojo / NUMERO_RAYOS_INDIRECTA;
            if ( seguirCamino == PATH_LEN)  verde = verde / NUMERO_RAYOS_INDIRECTA;
            if ( seguirCamino == PATH_LEN)  azul = azul / NUMERO_RAYOS_INDIRECTA;

            cIndir.set_values(rojo, verde, azul, NORMALIZAR_COLORES);
            inicial.sumar(cIndir);
        }
    }

    return inicial;
}

double operadorEscena::phong(Figura * figura, Punto x, Vector luz, Vector vista){
    Vector normal, R;
    double kd = AMBIENTE, La = 0.2, n = 4, ks = 0.3;
    luz.normalizar();
    vista.normalizar();
    normal = figura->normal(x);
    if ( productoEscalar(vista, normal) < 0 ) normal = valorPorVector(normal, -1);

    R = restaVectores(valorPorVector(normal, 2 * productoEscalar(luz, normal)), luz); //Ya normalizado
    double aux = productoEscalar(R, vista);

    return ks * (n + 2)/(2*M_PI) * pow(aux, n);
}

double operadorEscena::ward(Vector o, Vector i, Vector n, Punto p){
    double ps = 0.75, ax = 0.20, ay = 0.10, especular, exponente;
    Vector h, x, y;
    i.normalizar();
    o.normalizar();
    n.normalizar();
    if ( productoEscalar(i, n) < 0 ) n = valorPorVector(n, -1);

    //Se que es un poco ilegible, pero no es mas que la formula de Ward tal cual (version simplificada)
    h = valorPorVector(sumaVectores(i,o), 1.0 / (sumaVectores(i,o).modulo()));
    x = productoVectorial(n, i);
    y = productoVectorial(n, x);
    x.normalizar();
    y.normalizar();
    exponente = -(pow(productoEscalar(h,x)/ax,2) + pow(productoEscalar(h,y)/ay,2)) / pow(productoEscalar(h, n),2);
    especular = ps / (4*M_PI*ax*ay*sqrt(std::abs(productoEscalar(i,n) * productoEscalar(o, n)))) * pow(M_E, exponente);

    if ( especular < 0 ) especular = -especular;
    return especular;
}

Color operadorEscena::reboteEspecular(Figura * figura, Punto origen, Vector R, int numero){
    int min = -1, distancia;
    Color defecto;
    Rayo especular;
    Figura * choque;

    especular.set_values(origen, R);
    defecto.set_values(0,0,0, NORMALIZAR_COLORES);

    min = interseccion(especular, &choque);

    if ( min != -1){
            Punto puntoRender;
            Vector direccion = especular.getVector();
            puntoRender.set_values(origen.getX() + direccion.getX() * min, origen.getY() + direccion.getY() * min, 
                origen.getZ() + direccion.getZ() * min);
            
            return renderizar(puntoRender, choque, numero -1, origen, figura->getRefraccion(), false, 0);
        }
    else{
        return defecto;
    }
}

Color operadorEscena::refraccionEspecular(Figura * figura, Punto origen, Vector vista, double n1, double n2, int numeroRebotes){
    Vector normal = figura->normal(origen), refraccion;
    vista.normalizar();
    normal.normalizar();
    if ( productoEscalar(vista, normal) < 0 ) normal = valorPorVector(normal, -1); //Si no es la normal que queremos, la cambiamos de sentido.
    
    //Aplicamos la ley de snell
    double cosenoAngulo1 = (productoEscalar(vista, normal) / (normal.modulo() * vista.modulo()));
    double senoCAngulo2 = pow(n1/n2,2) * (1 - pow(cosenoAngulo1,2));
    Rayo rebote;
    Color defecto;
    int distancia, min = -1;
    Figura * choque;

    defecto.set_values(0,0,0, NORMALIZAR_COLORES);

    refraccion = sumaVectores(valorPorVector(vista, n1/n2), valorPorVector(normal, (n1/n2 * cosenoAngulo1 - sqrt(1-senoCAngulo2))));
    refraccion.normalizar();

    //Calculamos el nuevo rayo e intersectamos.
    rebote.set_values(origen, refraccion);
    min = interseccion(rebote, &choque);

    if ( min != -1){
        Punto puntoRender;
   
        puntoRender.set_values(origen.getX() + refraccion.getX() * min, origen.getY() + refraccion.getY() * min,
            origen.getZ() + refraccion.getZ() * min);
   
        return renderizar(puntoRender, choque, numeroRebotes -1, origen, n2, false, 0);
    }
    else{
        return defecto;
    }
}

double operadorEscena::interseccion(Rayo r, Figura ** choque){
    double distancia, min = -1;

    for (int i = 0; i < figuras.size(); i++){
        distancia = figuras[i]->intersectar(r);

        if ( distancia >= 0 ){

            if (( min == -1) | (distancia < min)){
                min = distancia;
                *choque = figuras[i];

                 if (figuras[i]->isBox()){
                    *choque = ((Box *) (figuras[i]))->store();
                } 
            }
        }
    }

    return min;
}
