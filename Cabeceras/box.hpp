/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una estructura de aceleración Bounding Volume Box
 */
#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_ 

#include <math.h>
#include <vector>

#include "punto.hpp"
#include "vector.hpp"
#include "plano.hpp"
#include "color.hpp"
#include "figura.hpp"
#include "triangulo.hpp"
#include "operadorVector.hpp"

class Box : public Figura{

    Figura ** figuras;
	Plano pl1, pl2, pl3, pl4, pl5, pl6;
    int capacidad, ocupados = 0, storaged;

	Plano crearPlano(Punto, Punto, Punto);

public:

	/*
     * Crea una caja definida con los puntos dados. Con capacidad para int objetos.
     */
    void set_values(Punto, Punto, Punto, Punto, Punto, Punto, int);

	/*
     * Devuelve la distancia a la que un Rayo r intercepta a un objeto de la bounding box f, -1 en caso de que no 
     * la intercepte.
     */
    double intersectar(Rayo);

    /**
     * No hace nada. (No es útil)
     */
    Vector normal(Punto);

    /**
     * Devuelve la ultima figura de la caja con la que se ha intersectado.
     */
    Figura * store();

    /**
     * Anyade una figura al volumen envolvente.
     */
    void meter(Figura *);

    /*
     * Libera la memoria dinamica utilizada por el volumen.
     */
    void free();

    /*
     * Devuelve cierto.
     */
    bool isBox();

    /*
     * Metodos de luces de area. (No hacen nada)
     */
    std::vector<Luz> getLuces();
    void setLuz(Luz);
    std::vector<Rayo> muestrearFotones(int n) {;}

};

#endif