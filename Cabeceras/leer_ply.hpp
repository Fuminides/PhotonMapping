/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Fichero que contiene las funciones para leer un fichero .ply que contiene unicamente triangulos.
 */

#ifndef FILE_PLY_
#define FILE_PLY_ 

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "triangulo.hpp"

/**
 * Lee el fichero .ply dado y carga los triangulos en un array pasado como parametro.
 */
void leer(string, Triangulo *);

/**
 * Salta al siguiente salto de linea del fichero.
 */
void nextLine(ifstream * istream, string * str);

/**
 * Lee un vertice del fichero.
 */
Punto leerPunto(ifstream * stream);

/**
 * Devuelve el numero de vertices en el fichero.
 */
int nTriangulos();

/**
 * Devuelve el numero de triangulos.
 */
int nPoligonos();

#endif