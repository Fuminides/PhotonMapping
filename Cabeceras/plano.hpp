/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a un plano.
 */
#ifndef PLANO_H_
#define PLANO_H_ 

#include "figura.hpp"
#include "vector.hpp"
#include "punto.hpp"
#include "rayo.hpp"
#include "operadorVector.hpp"

#include <math.h>

class Plano : public Figura
{

	Vector vNormal, vectorX, vectorY;
	Luz luces[5];
	Punto origen;
	double altura, anchura, d;

public:
	//Nota: Mismas especificaciones que para figura.hpp//

	/**
	 * Especifica primero el vector de altura y capacidad, y luego el vector anchura y modulo.
	 */
	void set_values(Punto, Vector, Vector, double, Vector, double);

	/*
	 * Devuelve la distancia a la que intersecta el rayo r la figura, o devuelve un valor negativo si no lo hace.
	 */
	double intersectar(Rayo);

	/*
     * Devuelve la normal de la figura dado un punto
     */
	Vector normal(Punto);

	bool isBox() { return false; }
	
	//----Luces de Area---
	void setLuz(Luz);
	std::vector<Luz> getLuces();

};

#endif