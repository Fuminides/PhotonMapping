/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una camara, con un origen, tres vectores, un tamano de imagen
 * y una resolucion.
 */
#ifndef COLOR_H_
#define COLOR_H_ 

#include <string>
#include <iostream>
#include <cmath>

class Color {
	double red, green, blue;
	double rojo, verde, azul;
	int RANGO = 255;
	bool normaliza = false;
	bool centinela = false;

	

	public:

		Color(){
			; //Es necesario, aunque no haga nada.
		}
		/*
		 * Especifica el RGB del color, y si se utiliza o no el modo contraste.
		 */
		void set_values(int,int,int, bool);
		void set_values_acum(int,int,int);


		bool iniciado() { return centinela; }
		/*
		 * Getters. (En formato de un bit)
		 */
		unsigned char splashR();
		unsigned char splashG();
		unsigned char splashB();

		/**
		 * Operaciones con los RGB. En caso de overflow dejan el valor maximo (1.0 * RANGO).
		 * No truncan en caso de que estemos usando el modo contraste.
		 */
		void multiplicar(double k);
		void sumar(Color); 

		/*
		 * Divide todos los canales entre max.
		 */ 
		void normalizar(double max);

		/*
		 * Devuelve el valor maximo en cualquiera de los canales RGB.
		 */
		double max();

		std::string to_string();


};

#endif