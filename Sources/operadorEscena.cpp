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
    crear(fotonMap);
    crear(fotonMapCaustics);
    if ( VIRTUAL ){
        trazar_luces();
    }
    else if ( PHOTON_MAPPING ){
        trazar_fotones();
    } 

    int num_luces = luces.size();
    int numero_pixels_totales = 1;
    if ( VIRTUAL ) numero_pixels_totales*=num_luces;

    defecto.set_values(0,0,0, NORMALIZAR_COLORES);
    std::list<Rayo> rayos = camara.trazarRayos();
    Color * pixels = (Color * ) malloc(sizeof(Color)*rayos.size());
    int rep = rayos.size();
    for ( int i = 0; i < rep; i++){
        Color color_misterios_de_c_mas_mas;
        color_misterios_de_c_mas_mas.set_values(0,0,0,NORMALIZAR_COLORES);
        pixels[i]= color_misterios_de_c_mas_mas;
    }

    int renderizados;
    float progress = 0.0;
    int concurentThreadsSupported = std::thread::hardware_concurrency();
    if ( concurentThreadsSupported == 0) concurentThreadsSupported = 1;
    int * i = (int*)calloc(sizeof(int),concurentThreadsSupported);
    cout << "Numero de cores en uso: " << concurentThreadsSupported << "\n";
    int threads_usados = 0;

    std::vector<future<int>> futures;
    futures.reserve(concurentThreadsSupported);
    if (VIRTUAL){
        for (luzEsperada = 0; luzEsperada < num_luces; luzEsperada++){
            for(int z = 0; z < concurentThreadsSupported; z++){
                futures.push_back(async(std::launch::async, &operadorEscena::execute_thread, this, z, concurentThreadsSupported, &(i[0]), rayos, pixels));
            }
            for (int cuenta = 0; cuenta < concurentThreadsSupported; ++cuenta)
            {
                futures[cuenta].wait();
            }
            cout << "Escena " << luzEsperada << " renderizada de " << num_luces << "\r";
        }
    }
    else{
        for(int z = 0; z < concurentThreadsSupported; z++){
            futures.push_back(async(std::launch::async, &operadorEscena::execute_thread, this, z, concurentThreadsSupported, &(i[0]), rayos, pixels));
        }
    }


    renderizados = 0;
    for(int count = 0; count < concurentThreadsSupported; count++){
        renderizados += i[count];
    }
    while( renderizados < rayos.size() ){
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        renderizados = 0;
        for(int count = 0; count < concurentThreadsSupported; count++){
            renderizados += i[count];
        }

        int copia = renderizados;
        progress= copia*1.0 / (rayos.size()*1.0);

        int barWidth = 70;

        std::cout << "[";
        int pos = barWidth * progress;
        for (int copia = 0; copia < barWidth; ++copia) {
            if (copia < pos) std::cout << "=";
            else if (copia == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " " <<  std::to_string(copia) << "/" << rayos.size() << " \r";
        std::cout.flush();
    }
            
    for (int cuenta = 0; cuenta < concurentThreadsSupported; ++cuenta)
    {
        futures[cuenta].wait();
    }
          
    std::cout << std::endl;         

    double areaPixel = (camara.getResX() * camara.getResY()) *1.0/ (rayos.size()*1.0);
    int fila = camara.getResX() / sqrt(areaPixel),
        columna = camara.getResY() / sqrt(areaPixel),
        nPixels = rayos.size();
    
    std::ofstream myfile;
    myfile.open ("example.ppm");
    myfile << "P6 " << fila << " " << columna << " 255\n";

    if ( NORMALIZAR_COLORES ){
        double max = 0;
        for ( int aux = 0; aux < rayos.size(); aux++){
            if ( pixels[aux].max() > max) max = pixels[aux].max();
        }
        if ( max > 255){
            for ( int aux = 0; aux < rayos.size(); aux++){
                pixels[aux].multiplicar(1.0/max);
                pixels[aux].multiplicar(255);
            }    
        }
    }
    myfile.close();

    unsigned char buffer[8088]; //El tamano del buffer ha sido escogido experimentando con tiempos de ejecucion
    FILE *fp;
    fp=fopen("example.ppm", "a");

    int contador = 0;

    for ( int j = 0; j <rep; j = j+1){
         if ( VIRTUAL ){
            pixels[j].multiplicar(1/(num_luces*1.0));
        }
        buffer[contador] =  pixels[j].splashR();
        buffer[contador+1] = pixels[j].splashG();
        buffer[contador+2] =  pixels[j].splashB();

       
        contador+=3;
        if ( contador == 8088 ){
            fwrite(buffer, sizeof(unsigned char), contador, fp);
            contador = 0;
        }
    }
    if ( contador > 0){
        fwrite(buffer, sizeof(unsigned char), contador, fp);
    }

    
    free(pixels);
    free(i);

    fclose(fp);
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
    int min, cuentaLuz = 0;
    //if (luzEsperada != 0 )cout << luzEsperada;
    for ( Luz luz: luces){
        //Luz directa
        //cout << "Esperada " << luzEsperada << " y la cuenta es " << cuentaLuz << "\n";
        if ((!VIRTUAL) | (VIRTUAL && (luzEsperada == cuentaLuz))){
            libre = true;
            Vector dirLuz = restaPuntos(luz.getOrigen(), p);
            Rayo puntoDirLuz;
            double dLuz = dirLuz.modulo();
            dirLuz.normalizar();
            puntoDirLuz.set_values(p, dirLuz);
            Figura * choque;

            min = -1;

            for (int i = 0; i < figuras.size(); i++){
                distancia = figuras[i]->intersectar(puntoDirLuz);

                if ( distancia >= 0 ){
                    if ((!figuras[i]->isLuz()) && (distancia < dLuz) && ((figura->getCoefRefraccion() == 0.0) | (figuras[i]->getCoefRefraccion()!=figura->getCoefRefraccion()))){ //Las luces de area no intersectan
                        min = distancia;
                    }
                }
            }

            if ( min > -1 ){
                libre = false;
            }

            if ( libre ){ //Si no hemos encontrado nada que tape la luz
                Color auxC = figura->getColor(), lcAux = luz.getColor(); 
                lcAux.multiplicar(1/lcAux.max());
                auxC.set_values(auxC.splashR()*(lcAux.splashR()/255),auxC.splashG()*(lcAux.splashG()/255),auxC.splashB()*(lcAux.splashB()/255), NORMALIZAR_COLORES);
                auxC.multiplicar(AMBIENTE/M_PI); //Mismo termino difuso para ambas BRDF.
                luz.atenuar(restaPuntos(p, luz.getOrigen()).modulo());

                if ( luz.getColor().max() > 0.005){ //Si no le llega la luz con un minimo de intensidad es como si no le llegara
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
                            auxC = refraccionEspecular(figura, p, restaPuntos(p, origenVista), figura->getRefraccion(), refraccionMedio, numeroRebotes);
                            auxC.multiplicar(figura->getCoefRefraccion());
                            inicial.sumar(auxC);
                        }
                    }

                    inicial.multiplicar(K_LUZ_DIR); //Solo para debug. (El k es 1)
                }
            }
        }
        cuentaLuz++;

    }

    if ( indirecto && !VIRTUAL){ //Si hay que calcular luz indirecta
        if (PHOTON_MAPPING){
            //Buscar los k mas proximos y dividir por el area
            std::vector<Foton> fotones, causticFoton;
            fotones = knearest(fotonMap, p, PROXIMOS); causticFoton = knearest(fotonMapCaustics, p ,PROXIMOS);
            double radio = lejano(fotones, p), radioCausticas = lejano(causticFoton, p);
            double R = 0.0, G = 0.0, B = 0.0;
            Color cIndir, cAux2;

            for(int i = 0; i < PROXIMOS; i++){
                Foton fotonAux = fotones[i];
                Color cAux = fotonAux.getColor();
               
                if ( figura->getBRDF() == 0){
                    bdrf = phong(figura, p, fotonAux.getDireccion() ,restaPuntos(origenVista,p)) + AMBIENTE/M_PI;   
                } 
                else if (figura->getBRDF() == 1){
                    bdrf = ward(restaPuntos(origenVista,p), fotonAux.getDireccion() , figura->normal(p), p) + AMBIENTE/M_PI;
                }

                cAux.multiplicar(bdrf);

                R+= cAux.splashR();
                G+= cAux.splashG();
                B+= cAux.splashB();

            }
            R = R / (M_PI*radio*radio);
            G = G / (M_PI*radio*radio);
            B = B / (M_PI*radio*radio);

            cIndir.set_values(R,G,B, NORMALIZAR_COLORES);

           R = 0.0; G = 0.0; B = 0.0;
            for(int i = 0; i < PROXIMOS; i++){
               Foton fotonAux = causticFoton[i];
               Color cAux = fotonAux.getColor();
               
                if ( figura->getBRDF() == 0){
                    bdrf = phong(figura, p, fotonAux.getDireccion() ,restaPuntos(origenVista,p))  + AMBIENTE/M_PI;   
                } 
                else if (figura->getBRDF() == 1){
                    bdrf = ward(restaPuntos(origenVista,p), fotonAux.getDireccion() , figura->normal(p), p) + AMBIENTE/M_PI;
                }
                cAux.multiplicar(bdrf);

                R+= cAux.splashR()* kernelGausiano(fotonAux, p, radioCausticas);
                G+= cAux.splashG()* kernelGausiano(fotonAux, p, radioCausticas);
                B+= cAux.splashB()* kernelGausiano(fotonAux, p, radioCausticas);
            }

            //R = R / (M_PI*radioCausticas*radioCausticas);
            //G = G / (M_PI*radioCausticas*radioCausticas);
            //B = B / (M_PI*radioCausticas*radioCausticas);
            cAux2.set_values(R,G,B, NORMALIZAR_COLORES);
            cIndir.sumar(cAux2);
            //if ( cIndir.max() > 0) cout << cIndir.to_string() << "\n";
 
            inicial.sumar(cIndir);


        }
        else{
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
                                bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p)) + AMBIENTE/M_PI;   
                            } 
                            else if (figura->getBRDF() == 1){
                                bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p) + AMBIENTE/M_PI;
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
                    /*for ( i = 0; (min == -1) && (i <2); i++){
                        Vector vect = montecarlo.calcularw().front();
                        rayo.set_values(p, vect);
                        min = interseccion(rayo, &figuraAux);
                    }*/

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
                                    bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p)) + AMBIENTE/M_PI;   
                                } 
                                else if (figura->getBRDF() == 1){
                                    bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p) + AMBIENTE/M_PI;
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
                                bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p)) + AMBIENTE/M_PI;   
                            } 
                            else if (figura->getBRDF() == 1){
                                bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p) + AMBIENTE/M_PI;
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
    double cosenoAngulo1 = (productoEscalar(vista, normal)), distancia;
    
    Rayo rebote;
    Color defecto;
     int min = -1;
    Figura * choque;
    defecto.set_values(0,0,0, NORMALIZAR_COLORES);

    if ( cosenoAngulo1 > 0.0) {
        normal = valorPorVector(normal, -1); //Si no es la normal que queremos, la cambiamos de sentido.
    }
    else{
        double nAux = n1;
        n1 = n2;
        n2 = nAux;
        cosenoAngulo1 = -cosenoAngulo1;
    }

    //Aplicamos la ley de snell
    float cosT = 1.0f - pow(n1 / n2, 2.0f) * (1.0f - pow(cosenoAngulo1, 2.0f));
    cosT = sqrt(cosT);
    refraccion = sumaVectores(valorPorVector(vista , (n1 / n2) ) , valorPorVector(normal , ((n1 / n2) * cosenoAngulo1 - cosT)));

    rebote.set_values(origen, refraccion);
    min =-1;

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
            if ((( min == -1) | (distancia < min))){
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

double operadorEscena::interseccionLuz(Rayo r, Figura ** choque){
    double distancia, min = -1;

    for (int i = 0; i < figuras.size(); i++){
        distancia = figuras[i]->intersectar(r);

        if ( distancia >= 0 ){

            if ((figuras[i]->getCoefRefraccion()==0.0) && (( min == -1) | (distancia < min))){
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

int operadorEscena::execute_thread(int id, int intervalo,  int * cuenta, list<Rayo> rayos, Color * pixels){
    Figura *choque;
    Color defecto;
    defecto.set_values(0,0,0, NORMALIZAR_COLORES);
    int i = id, z = 0;

    for (Rayo rayo: rayos){
        if (z==i){
            double min = -1;

            min = interseccion(rayo, &choque);

            if ( min != -1){
                if ( !choque->isLuz() ) {
                    Punto puntoRender, origenRayos = camara.getPosicion();
                    Vector direccion = rayo.getVector();
                    puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                        origenRayos.getZ() + direccion.getZ() * min);
                     Color colorGen = renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), REFRACCION_MEDIO, true, PATH_LEN);
                   // if (luzEsperada > 3 && (pixels[z].max() > 1))cout << "Color gen: " << colorGen.to_string() << " pixels[z]: " << pixels[z].to_string() << "\n";
                    pixels[z].sumar_escenas(colorGen);

                }
                else{
                    Color colorAux = choque->getColor();
                    pixels[z].sumar_escenas(colorAux);
                }
            }
            else{
                    pixels[z].sumar_escenas(defecto);
            }

            cuenta[id] ++;
            i += intervalo;
        }

        z++;
    }

    return 1;

}

void operadorEscena::trazar_fotones(){
    int restantes = FOTONES, restantesCausticas = FOTONES_CAUSTICA;
    int iteracion = 1;
    double potenciaTotal = 0, potenciaTotalArea = 0;
    for (Luz luz: luces){
        if (!luz.getArea() ) potenciaTotal += luz.getPotencia();
        cout << potenciaTotal;
    }
    for (Luz luz: luces){
        if (luz.getArea() ) potenciaTotalArea += luz.getPotencia()/5.0; //5 es el numero de luces puntuales por luz de area
    }
    while ((restantes > 0) | (restantesCausticas > 1)){
        //Primero sampleamos las luces puntuales
        if ( potenciaTotal > 0){
            for(Luz luz:luces){
                double porcentaje = luz.getPotencia() / potenciaTotal;
                int numeroLuces = porcentaje * (FOTONES+FOTONES_CAUSTICA);
                int viejo = restantes+restantesCausticas;
                while ( restantesCausticas+restantes > viejo - numeroLuces){
                    cout << "Fotones por mapear restantes (Iteracion: "<<iteracion++<<") " <<": Normales ("<<FOTONES<<"): " << -(restantes-FOTONES) << " , Causticas ("<<(FOTONES_CAUSTICA-2)<<"): " << -(restantesCausticas-FOTONES_CAUSTICA) << "\r";
                    if (!luz.getArea()){
                        std::vector<Rayo> rayosLuz = luz.muestrearFotones();
                        luz.setPotencia(luz.getPotencia()*(FOTONES+FOTONES_CAUSTICA) / (numeroLuces*1.0));
                        for(Rayo rayo : rayosLuz){
                            trazarCaminoFoton(rayo, luz, PATH_LEN, &restantes, &restantesCausticas, false);
                        }
                    }
                }
            }
        }
        
        //Luego, las luces de area
        if ( potenciaTotalArea > 0){
            for(Figura * figura: figuras){
                double porcentaje = figura->getLuces().at(0).getPotencia() / potenciaTotalArea;
                int numeroLuces = porcentaje * (FOTONES+FOTONES_CAUSTICA);
                int viejo = restantes+restantesCausticas;
                while ( restantesCausticas+restantes > viejo - numeroLuces){
                    cout << "Fotones por mapear restantes (Iteracion: "<<iteracion++<<") " <<": Normales ("<<FOTONES<<"): " << -(restantes-FOTONES) << " , Causticas ("<<(FOTONES_CAUSTICA-2)<<"): " << -(restantesCausticas-FOTONES_CAUSTICA) << "\r";
                    if (figura->isLuz()){
                        std::vector<Rayo> rayosLuz = figura->muestrearFotones();
                        Luz luz = figura->getLuces().at(0);
                        luz.setPotencia(luz.getPotencia()*(FOTONES+FOTONES_CAUSTICA) / numeroLuces);
                        for(Rayo rayo : rayosLuz){
                            luz.setOrigen(rayo.getOrigen());
                            trazarCaminoFoton(rayo, luz, PATH_LEN, &restantes, &restantesCausticas, false);
                        }
                    }
                }
            }
        }
    }
    cout << "\nMapas de fotones creados.\n";
    cout.flush();
}

void operadorEscena::trazar_luces(){
    int restantes = LUCES;
    int iteracion = 1;
    double potenciaTotal = 0, potenciaTotalArea = 0;
    for (Luz luz: luces){
        if (!luz.getArea() ) potenciaTotal += luz.getPotencia();
        cout << potenciaTotal;
    }
    for (Luz luz: luces){
        if (luz.getArea() ) potenciaTotalArea += luz.getPotencia()/5.0; //5 es el numero de luces puntuales por luz de area
    }
    while (restantes > 0){
        //Primero sampleamos las luces puntuales
        if ( potenciaTotal > 0){
            for(Luz luz:luces){
                double porcentaje = luz.getPotencia() / potenciaTotal;
                int numeroLuces = LUCES / luces.size();
                int viejo = restantes;
                while ( restantes > viejo - numeroLuces){
                    cout << "Luces por crear restantes: " << restantes << "\n";
                    if (!luz.getArea()){
                        std::vector<Rayo> rayosLuz = luz.muestrearFotones();
                        for(Rayo rayo : rayosLuz){
                            trazarCaminoLuz(rayo, luz, PATH_LEN, &restantes);
                        }
                    }
                }
            }
        }
        
        //Luego, las luces de area
        if ( potenciaTotalArea > 0){
            for(Figura * figura: figuras){
                double porcentaje = figura->getLuces().at(0).getPotencia() / potenciaTotalArea;
                int numeroLuces = porcentaje * LUCES;
                int viejo = restantes;
                while ( restantes > viejo - numeroLuces){
                    cout << "Luces por crear restantes (Iteracion: "<<iteracion++<<") " <<LUCES << "\r";
                    std::vector<Rayo> rayosLuz = figura->muestrearFotones();
                    for(Rayo rayo : rayosLuz){
                        trazarCaminoLuz(rayo, figura->getLuces().at(0), PATH_LEN, &restantes);
                    }
                }
            }
        }
    }
    cout << "\nLuces creadas.\n";
    cout.flush();
}

void operadorEscena::trazarCaminoFoton(Rayo r, Luz l, int profundidad, int * normales, int * causticas, bool caustica){
    Figura * choque;
    Montecarlo montecarlo;
    Punto pInterseccion;
    Punto origen = r.getOrigen();
    double min = interseccion(r, &choque);

    if(min > -1){
        //cout << "Ref: " 
        l.atenuar(min);
        if( (profundidad != PATH_LEN) && (l.getColor().max()*255 > 0) && (choque->getReflejo() == 0)){
           //Guardamos
            Foton foton;
            Vector direccion = r.getVector();
            pInterseccion.set_values(origen.getX() + direccion.getX()*min,origen.getY()+direccion.getY()*min,origen.getZ()+direccion.getZ()*min);
            foton.set_values(valorPorVector(direccion, -1) , l.getColor(), pInterseccion);
            if ( caustica && (*causticas > 0) ){
                //cout << l.getColor().to_string() << "\n";
                anyadir(fotonMapCaustics,foton);
                *causticas+=-1;
            }
            else if ( *normales > 0 ) {
                //cout << l.getColor().to_string() << "\n";
                anyadir(fotonMap, foton);
                *normales+=-1;
            }
  
        }
        //Seguimos el camino
        if( profundidad > 0){
            //Ruleta Rusa
            Vector azar;
            int umbral = 7, difuso = (int) (umbral*choque->getCoefRefraccion());
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            int suerte = ((int) (dist(mt)*10));

            if ( suerte < (umbral - difuso)){ //Rebote
                Vector azar;
                Punto aux;
                aux.set_values(0,0,0);
                Vector direccion = r.getVector(); 

                pInterseccion.set_values(origen.getX() + direccion.getX()*min,origen.getY()+direccion.getY()*min,origen.getZ()+direccion.getZ()*min);
          
                Vector vNormal = choque->normal(pInterseccion);
                if ( productoEscalar(vNormal, direccion) > 0 ) vNormal = valorPorVector(vNormal,-1);
                Montecarlo montecarlo;

                montecarlo.set_values(restaPuntos(pInterseccion,aux),vNormal, 1);

                do{
                    //direccion.set_values(dist(mt),dist(mt),dist(mt));
                    direccion = montecarlo.calcularw().front(); //Creamos un vector que este en la direccion que queremos.
                    //cout << "Punto: " << pInterseccion.to_string() << ", Vector: " << direccion.to_string() << ", Normal: " << choque->normal(pInterseccion).to_string() << "\n";
                } while (productoEscalar(direccion, vNormal) <= 0);

                direccion.normalizar();

                Color cAux = choque->getColor();
                cAux.multiplicar(l.getPotencia());

                if (choque->getBRDF() == 0 )  cAux.multiplicar(phong(choque, pInterseccion, valorPorVector(r.getVector(),-1), direccion)+ AMBIENTE/M_PI);
                else if (choque->getBRDF() == 1 ) cAux.multiplicar(ward(direccion, valorPorVector(r.getVector(),-1), vNormal,pInterseccion)+ AMBIENTE/M_PI);

                l.setColor(cAux);
                r.set_values(pInterseccion, direccion);
                l.setOrigen(r.getOrigen());

                trazarCaminoFoton(r,l,profundidad-1, normales, causticas, false);
            }
            else if ( suerte < umbral){ //refraccion
                Vector direccion = r.getVector();
                pInterseccion.set_values(origen.getX() + direccion.getX()*min,origen.getY()+direccion.getY()*min,origen.getZ()+direccion.getZ()*min);
                Vector normal = choque->normal(pInterseccion), refraccion;
                Vector vista = r.getVector();
                vista.normalizar();
                normal.normalizar();
                double n2 = REFRACCION_MEDIO, n1 = choque->getRefraccion();
                Rayo rebote;

                double cosenoAngulo1 = productoEscalar(vista, normal);
                if ( cosenoAngulo1 > 0.0) {
                    normal = valorPorVector(normal, -1); //Si no es la normal que queremos, la cambiamos de sentido.
                }
                else{
                    n1 = REFRACCION_MEDIO;
                    n2 = choque->getRefraccion();
                    cosenoAngulo1 = -cosenoAngulo1;
                }

                //Aplicamos la ley de snell
                float cosT = 1.0f - pow(n1 / n2, 2.0f) * (1.0f - pow(cosenoAngulo1, 2.0f));
                cosT = sqrt(cosT);
                refraccion = sumaVectores(valorPorVector(vista , (n1 / n2) ) , valorPorVector(normal , ((n1 / n2) * cosenoAngulo1 - cosT)));
                refraccion.normalizar();
                rebote.set_values(pInterseccion, refraccion);
                
                l.setOrigen(pInterseccion);
                Color cAux = l.getColor();
                //cAux.multiplicar(choque->getCoefRefraccion());
                l.setColor(cAux);
                trazarCaminoFoton(rebote, l, profundidad-1, normales, causticas, true);
            }
            //Si no, el rayico a casa que hace frio
        }
    }
}

void operadorEscena::trazarCaminoLuz(Rayo r, Luz l, int profundidad, int * normales){
    Figura * choque;
    Montecarlo montecarlo;
    Punto pInterseccion;
    Punto origen = r.getOrigen();
    double min = interseccion(r, &choque);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Color inicial = l.getColor();

    if((min > -1) && (*normales>0)){
        Color cAux = choque->getColor();
        //cAux.multiplicar(l.getPotencia());
        l.setColor(cAux);
        //l.atenuar(min);
        Punto aux;
        aux.set_values(0,0,0);
        Vector direccion = r.getVector(); 

        pInterseccion.set_values(origen.getX() + direccion.getX()*min,origen.getY()+direccion.getY()*min,origen.getZ()+direccion.getZ()*min);
       
        

        if(l.getColor().max()*255 > 20){
           //Guardamos
            if ( (l.getColor().splashB() == l.getColor().splashG()) && (l.getColor().splashG() == l.getColor().splashR()) ){
                double suerte = dist(mt);
                if ( suerte > 0.1 ){ anyadirLuz(l); *normales = *normales -1;             cout << l.getColor().to_string() << "\n";}
            }
            else{
                anyadirLuz(l);
                *normales= *normales - 1;
                cout << l.getColor().to_string() << "\n";
            }
        }
        if( profundidad > 0){
            Vector azar;
            int difuso = (int) (10*choque->getCoefRefraccion());
            int suerte = ((int) (dist(mt)*10));
            if ( suerte > difuso){
                Vector vNormal = choque->normal(pInterseccion);
                if ( productoEscalar(vNormal, direccion) > 0 ) vNormal = valorPorVector(vNormal,-1);
                Montecarlo montecarlo;
                montecarlo.set_values(restaPuntos(pInterseccion,aux),vNormal, 1);

                do{
                    direccion = montecarlo.calcularw().front(); //Creamos un vector que este en la direccion que queremos.
                } while (productoEscalar(direccion, vNormal) <= 0);

                direccion.normalizar();
                r.set_values(pInterseccion, direccion);
                l.setOrigen(r.getOrigen());

                trazarCaminoLuz(r,l,profundidad-1, normales);
            }
            else{
                Vector direccion = r.getVector();
                pInterseccion.set_values(origen.getX() + direccion.getX()*min,origen.getY()+direccion.getY()*min,origen.getZ()+direccion.getZ()*min);
                Vector normal = choque->normal(pInterseccion), refraccion;
                Vector vista = r.getVector();
                vista.normalizar();
                normal.normalizar();
                double n2 = REFRACCION_MEDIO, n1 = choque->getRefraccion();
                Rayo rebote;

                double cosenoAngulo1 = productoEscalar(vista, normal);
                if ( cosenoAngulo1 > 0.0) {
                    normal = valorPorVector(normal, -1); //Si no es la normal que queremos, la cambiamos de sentido.
                }
                else{
                    n1 = REFRACCION_MEDIO;
                    n2 = choque->getRefraccion();
                    cosenoAngulo1 = -cosenoAngulo1;
                }

                //Aplicamos la ley de snell
                float cosT = 1.0f - pow(n1 / n2, 2.0f) * (1.0f - pow(cosenoAngulo1, 2.0f));
                cosT = sqrt(cosT);
                refraccion = sumaVectores(valorPorVector(vista , (n1 / n2) ) , valorPorVector(normal , ((n1 / n2) * cosenoAngulo1 - cosT)));
                refraccion.normalizar();
                rebote.set_values(pInterseccion, refraccion);
                l.setOrigen(pInterseccion);
                l.setColor(inicial);

                trazarCaminoLuz(rebote,l,profundidad-1, normales);
            }
        }    
    }
}


double operadorEscena::lejano(std::vector<Foton> fotones, Punto p){
    double resultado = 0.0;

    for(int i = 0; i < PROXIMOS; i++){
        Foton foton = fotones[i];
        Vector dist = restaPuntos(foton.getPosicion(), p);
        double modulo = dist.modulo();

        if (resultado < modulo){
            resultado = modulo;
        }
    }

    return resultado;
}

double operadorEscena::kernelGausiano(Foton foton, Punto p, double maxD){
    double alfa = 0.918, beta = 1.953;
    Punto pFoton = foton.getPosicion();
    Vector vDistancia = restaPuntos(pFoton, p);
    double distancia = vDistancia.modulo(),
        exponente = pow(distancia,2)/(2*pow(maxD,2));
    exponente = -beta * exponente;
    double numerador = 1 - pow(M_E, exponente);
    double denominador = 1 - pow(M_E, -beta);

    return alfa * abs(1 - numerador/denominador);
}