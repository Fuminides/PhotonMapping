/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet
 *
 * Clase que representa a los vectores.
 */
#include "vector.hpp"

#include <cmath>

bool Vector::esVector() { return true; } 

double Vector::modulo() { return sqrt(pow(valores[0], 2) + pow(valores[1], 2) + pow(valores[2], 2)); }

void Vector::normalizar(){
	double module = modulo();
	set_values(valores[0]/module, valores[1]/module, valores[2]/module);
}

void Vector::multiplicar(double k){
	valores[0] = valores[0] * k;
	valores[1] = valores[1] * k;
	valores[2] = valores[2] * k;
}


