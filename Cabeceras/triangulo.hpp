/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a un triangulo en el espacio.
 */
#ifndef TRIANGULO_H_
#define TRIANGULO_H_

#include "figura.hpp"
#include "operadorVector.hpp"
#include "plano.hpp"

#include <string>
#include <vector>

#define EPSILON 0.01

class Triangulo : public Figura {

	Punto p1, p2, p3;
	Vector v1, v2, v3;
	Luz foco;

	public:
		/*
		 * Define el triangulo con sus tres vertices.
		 */
		void set_values(Punto, Punto, Punto);

		/*
		 * Devuelve la distancia > 0 si el rayo intersecta con el triangulo.
		 */
		double intersectar(Rayo);

		/*
		 * Devuelve la normal del triangulo en un punto.
		 */
		Vector normal(Punto);

		/*
		 * Multiplica todos los vertices del triangulo por el numero dado.
		 */
		void multiplicar(double);

		/*
		 * Mueve el triangulo en la direccion dada.
		 */
		void desplazar(Vector);

		std::string to_string();
		bool isBox() { return false; }

		//----Luces de area----
		std::vector<Luz> getLuces();
		void setLuz(Luz);
		std::vector<Rayo> muestrearFotones(){;}
};

#endif