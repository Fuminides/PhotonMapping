/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a luz, que es un punto con un color y una potencia.
 */
#ifndef LUZ_H
#define LUZ_H

#include "color.hpp"
#include "rayo.hpp"

class Luz  {
	Color color;
	Punto origen;
	double potencia;

public:
	void set_values(Punto, Color, double);

	/*
	 * Getters.
	 */
	Color getColor();
	Punto getOrigen();
	double getPotencia();

	/*
	 * Disminuye la potencia de la luz en funcion de la distancia dada.
	 */
	void atenuar(double);

};

#endif