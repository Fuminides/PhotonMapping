/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que ofrece metodos para operar con vectores y puntos.
 */
#ifndef operadorVector_h_
#define operadorVector_h_ 

#include "vector.hpp"
#include "punto.hpp"
#include "matriz.hpp"
#include "luz.hpp"

/**
 * Multiplica una matriz por columnas por un vector columna
 */
Vector matrizPorVector(Matriz m, Vector v);

/**
 * Multiplica una matriz por columnas por un punto
 */
Punto matrizPorPunto(Matriz m, Punto v);

/**
 * Multiplica dos vectores de forma matricial,
 * es equivalente al producto escalar.
 */
double multiplicacionMatricial(Vector v1, Vector v2);
double productoEscalar(Vector v1, Vector v2);

/**
 * Operaciones basicas de vectores
 */
Vector sumaVectores(Vector v1,Vector v2);
Vector restaVectores(Vector v1,Vector v2);

/**
 * Devuelve el vector que describe la direccion de la recta
 * que delimitan dos puntos.
 */
Vector restaPuntos(Punto p1,Punto p2);

/**
 * Devuelve el producto vectorial de dos vectores 3D.
 */
Vector productoVectorial(Vector v1, Vector v2);

/**
 * Multiplica un vector por un escalar
 */
Vector valorPorVector(Vector v,double alfa);

/**
 * Suma un vector a un punto (lo mismo que mover al punto en la direccion del vector).
 */
Punto sumaPuntoVector(Punto p,Vector v);

/**
 * Suma las coordenadas de dos puntos.
 */
Punto sumaPuntos(Punto p, Punto p2);

#endif