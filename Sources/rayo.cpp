#include "rayo.hpp"
#include "punto.hpp"


void Rayo::set_values(Punto p, Vector d){
	origen = p;
	v = d;
}
void Rayo::setOrigen(Punto o){
        origen=o;
}
void Rayo::setVector(Vector d){
        v=d;    
}
Punto Rayo::getOrigen(){
        return origen;
}
Vector Rayo::getVector(){
        return v;
}

