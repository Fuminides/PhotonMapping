/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una matriz 4x4.
 */
#ifndef matriz_h_
#define matriz_h_ 

#include "vector.hpp"
#include <string>


class Matriz {

	Vector vectores[4];

public:

	void set_values(Vector _v1, Vector _v2, Vector _v3, Vector _v4);
	void set_values(Vector _v1, Vector _v2, Vector _v3);
    void set_valuesColum(Vector _v1, Vector _v2, Vector _v3,Vector _v4);

	/*
	 * Getters.
	 */
	Vector getVector(int indice);
	Vector getVector1();
	Vector getVector2();
	Vector getVector3();
	Vector getVector4();
	double get(int x, int y);

	/*
	 * Setters.
	 */
	void setVector(int, Vector v);
	void setVector1(Vector v);
	void setVector2(Vector v);
	void setVector3(Vector v);
	void setVector4(Vector v);
    
	std::string toString();
};

#endif