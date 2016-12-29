/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet
 *
 * Clase que representa a los vectores.
 */
#include "punto.hpp"


bool Punto::esVector() { return false; } 

bool Punto::equals(Punto p){
	if ((p.getX() == getX()) && (p.getY() == getY()) && (p.getZ() == getZ())) {
		return true;
	}
	else{
		return false;
	}
}
