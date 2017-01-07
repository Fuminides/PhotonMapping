#ifndef FOTON_H
#define FOTON_H 

#include "vector.hpp"
#include "color.hpp"
#include "punto.hpp"

class Foton{

	Vector direccion;
	Color color;
	Punto posicion;

public:

	void set_values(Vector _direccion, Color _color, Punto _posicion){
		direccion = _direccion;
		color = _color;
		posicion = _posicion;
	}

	Vector getDireccion() { return direccion;}

	Color getColor(){return color;}

	Punto getPosicion(){return posicion;}


};


#endif