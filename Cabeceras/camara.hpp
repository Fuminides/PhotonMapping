/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una camara, con un origen, tres vectores, un tamano de imagen
 * y una resolucion.
 */
#ifndef camara_h_
#define camara_h_

#include "vector.hpp"
#include "punto.hpp"
#include "rayo.hpp"
#include "operadorVector.hpp"

#include <iostream>
#include <list>

class Camara {

	int resX, resY, pixels;
	double distancia;
	Punto posicion, plano;
	Vector ejes[3];
	

	Rayo trazarRayo(Punto p);
	
	public:

		void set_values(Punto, Vector, Vector, Vector, int, int, int);

		/*
		 * Getters.
		 */
		Vector getVX();
		Vector getVY();
		Vector getVZ();
		Punto getPosicion();
		double getDistancia();
		int getResX();
		int getResY();
		int getPixels();

		/*
		 * Setters.
		 */
		void setVX(Vector);
		void setVY(Vector);
		void setVZ(Vector);
		void setPosicion(Punto);

		/*
		 * Devuelve el conjunto de rayos con origen en la camara y que pasan por cada uno de los pixeles
		 * por el centro.
		 */
		std::list<Rayo> trazarRayos();

		
};

#endif