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

#include <vector>
#include <random>


class Luz  {
	Color color;
	Punto origen;
	double potencia;
	bool area = false;

public:
	void set_values(Punto, Color, double);

	/*
	 * Getters.
	 */
	Color getColor();
	Punto getOrigen();
	double getPotencia();

	/*
	 * Setters
	 */
	void setColor(Color c){ color = c; }
	void setOrigen(Punto p) { origen = p; }

	/*
	 * Disminuye la potencia de la luz en funcion de la distancia dada.
	 */
	void atenuar(double);

	/*
	 * Indica si la luz puntual pertenece a una luz de área o no. (Por defecto, no)
	 */
	bool getArea(){ return area; }
	void esArea() { area = true; }

	std::vector<Rayo> muestrearFotones();

	void setPotencia(double d) { potencia = d; }

};

#endif