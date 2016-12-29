/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a un punto en el espacio cartesiano.
 */
#ifndef punto_h_
#define punto_h_

#include "objeto_cartesiano.hpp"

 class Punto : public ObjetoCartesiano {
 public:
 	bool esVector();
 	bool equals(Punto);
 };

 #endif