/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a un vector.
 */
#ifndef vector_h_
#define vector_h_

#include "objeto_cartesiano.hpp"

 class Vector : public ObjetoCartesiano {
 	
 public:
 	
 	bool esVector();

 	/**
 	 * Devuelve el modulo del vector.
 	 */
	double modulo();

	/**
	 * Normaliza el vector
	 */
	void normalizar();

	/**
	 * Multiplica las coordenadas del vector por un numero dado.
	 */
	void multiplicar(double);
 };

 #endif