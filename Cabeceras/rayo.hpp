/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a un rayo, que consiste en un punto y una direccion.
 */
#ifndef RAYO_H
#define RAYO_H

#include "punto.hpp"
#include "vector.hpp"

class Rayo{

        Punto origen;
        Vector v;

    public:
    	void set_values(Punto, Vector);
    	/*
		 * Setters.
		 */
        void setOrigen(Punto);
        void setVector(Vector);
		/*
		 * Getters.
		 */
        Punto getOrigen();
        Vector getVector();

};

#endif 
