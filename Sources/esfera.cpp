#include "esfera.hpp"

    /*
     * Devuelve el radio de la esfera
     */ 
    double Esfera::getRadio(){
        return radio;
    }
    /*
     * Asigna un valor al radio 
     */
    void Esfera::setRadio(double r){
        radio=r;
    }
    /*
     * Devuelve el origen de la esfera en el plano
     */
    Punto Esfera::getOrigen(){
        return origen;
    }
    /*
     * Asigna el valor del origen de la esfera
     */
    void Esfera::setOrigen(double x, double y, double z){
            origen.set_values(x,y,z);
            //color.set_values(0,0,0);
    }
    /*
     * Devuelve la superficie de la esfera
     */
    double Esfera::superficie(){
        return radio * radio * M_PI;
    }

    /*
     * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
     * la intercepte
     */
    double Esfera::intersectar(Rayo r){
        Punto O= r.getOrigen();
        Vector D = r.getVector();
        Punto C= getOrigen();

        double b = puntoPorVector(O, D);
        b -= puntoPorVector(C, D);
        b = b*2;

        double c = puntoPorPunto(O, O);
        c += puntoPorPunto(C, C);
        c -= 2 * puntoPorPunto(C, O);
        c -= radio * radio;

        double aux = b * b - 4 * c;
        if (aux * aux < UMBRAL) aux = 0;
        
        if ( aux < 0.0 ){
            return -1.0;
        }

        double t1 = 0.0 - b + sqrt(aux);
        t1 = t1 / 2.0;
        double t2 = 0.0 - b - sqrt(aux);
        t2 = t2 / 2.0;

        if ( (t1 > 0.0) && (t2 > 0.0) ){
            if ( t1 < t2){
                if ( t1*t1 > UMBRAL ){
                    return t1;
                }
                else{
                    return t2;
                }
            }
            else if ( t1 > t2){
                if (t2 * t2 > UMBRAL){
                    return t2;
                }
                else{
                    return t1;
                }
            }
            else{
                if ( t1*t1 > UMBRAL){
                    return t1;
                }
                else{
                    return -1;
                }
            }
        }
        else if ( (t1 > 0.0) ){
            if (t1 * t1 > UMBRAL){
                    return t1;
            }
            else{
                return -1;
            }
        }
        else if ( (t2 > 0.0) ){
            if ( t2 * t2 > UMBRAL){
                return t2;
            }
            else{
                return -1;
            }
        } 

        return -1;        
    }

double Esfera::puntoPorPunto(Punto p1, Punto p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ();
}

double Esfera::puntoPorVector(Punto p1, Vector p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ(); 
}

Vector Esfera::normal(Punto p){
    Vector v;
    v = restaPuntos(p, origen);
    v.normalizar();
    return v;
}

Luz generarLuz(Punto abajo, Vector aux, Luz _l, Punto origen, double radio){
    aux = restaPuntos(abajo, origen);
    aux.normalizar();
    aux = valorPorVector(aux, radio);
    abajo = sumaPuntoVector(origen, aux);

    Luz lNueva;
    lNueva.set_values(abajo, _l.getColor(), _l.getPotencia());

    return lNueva;
}

void Esfera::setLuz(Luz _l){
    luz = true;
    color = _l.getColor();
    Punto abajo, arriba, derecha, izquierda, atras, delante;
    Vector aux;

    abajo.set_values(0,-1,0);
    arriba.set_values(0,1,0);
    derecha.set_values(0,0,1);
    izquierda.set_values(0,0,-1);
    atras.set_values(-1,0,0);
    delante.set_values(1,0,0);

    luces[0] = generarLuz(abajo, aux, _l, origen, radio);
    luces[1] = generarLuz(arriba, aux, _l, origen, radio);
    luces[2] = generarLuz(derecha, aux, _l, origen, radio);
    luces[3] = generarLuz(izquierda, aux, _l, origen, radio);
    luces[4] = generarLuz(atras, aux, _l, origen, radio);
    luces[5] = generarLuz(delante, aux, _l, origen, radio);
}

std::vector<Luz> Esfera::getLuces(){
    std::vector<Luz> v(std::begin(luces), std::end(luces));    
    return v;
}

