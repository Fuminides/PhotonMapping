/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una escena.
 */
#ifndef OPERADORESCENA_H
#define OPERADORESCENA_H

#include "rayo.hpp"
#include "esfera.hpp"
#include "camara.hpp"
#include "box.hpp"
#include "montecarlo.hpp"
#include "kdtree.hpp"
#include "luz.hpp"
#include "color.hpp"
#include "figura.hpp"
#include "foton.hpp"
#include "plano.hpp"
#include "punto.hpp"
#include "vector.hpp"

#include <vector>
#include <string>
#include <list>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cmath> 
#include <future>
#include <random>

class operadorEscena
{
    
private:

    
    //Coeficientes de ajuste (Usados para debug)
    const double AMBIENTE = 1.0;
    const double REFRACCION_MEDIO = 1.0;
    const double K_LUZ_DIR = 1.0;

    //Caminos especulares
    const int NUMERO_REBOTES  = 4;
    
    //Indirecta
    const int PATH_LEN = 4;
    const double K_LUZ_INDIR = 0.0 / (NUMERO_RAYOS_INDIRECTA + 0.000000000001);

    //Numero de fotones por mapa
    const int FOTONES = 1000;
    const int FOTONES_CAUSTICA = 1500;
    const int PROXIMOS = 50;

    //Numero de luces virtuales
    const int LUCES = 1000;

    int luzEsperada;

    Camara camara;
    std :: vector < Figura * > figuras;
    std :: vector < Luz > luces;
    kdtree fotonMap;
    kdtree fotonMapCaustics;




    /**
     * Calcula la BDRF de Phong en el punto dado de la figura dada.
     */
    double phong(Figura * figura, Punto x, Vector entrada, Vector salida);

     /**
     * Calcula la BDRF de Ward en el punto dado de la figura dada.
     */
    double ward(Vector o, Vector i, Vector n, Punto x);

    /** 
     * Dado un rayo, devuelve la distancia a la que choca con un figura.
     * @choque variable de salida con un puntero a la figura intersectada.
     */
    double interseccion(Rayo r, Figura ** choque);
    double interseccionLuz(Rayo , Figura ** );

    void trazarCaminoFoton(Rayo r, Luz l, int profundidad, int *, int *, bool);
    void trazarCaminoLuz(Rayo, Luz, int, int *);

    double lejano(std::vector<Foton> fotones, Punto p);
    double kernelGausiano(Foton, Punto, double);

public:

    //Contrastes
    const bool NORMALIZAR_COLORES = false; //Solo funciona bien en alguna escena muy concreta con muchos fogonazos
    int NUMERO_RAYOS_INDIRECTA = 0;
    bool PATH_TRACING = false;
    bool PHOTON_MAPPING = true;
    bool VIRTUAL = false;

    /**
     * Incluye una camara nueva en la escena
     */
    void setCamara(Camara); 

    /*
     * Añade una esfera al array de esferas de la escena
     */
    void anyadirFigura(Figura * f);

    /*
     * Anyade una fuente de luz a la escena
     */
    void anyadirLuz(Luz);

    /**
     * Dibuja la escena
     */
    void dibujar();

    /**
     * Renderiza el color de un punto
     */
    Color renderizar(Punto c, Figura * figura, int, Punto, double, bool, int);

    /**
     * Define un rebote especular y lo calcula con el numero dado de rebotes de profundidad.
     */
    Color reboteEspecular(Figura * figura, Punto origen, Vector R, int numero);

    /**
     * Calcula una refraccion especular en el punto desde el punto vista dado, con los dos coeficientes de refraccion de ambos medios.
     */
    Color refraccionEspecular(Figura * figura, Punto origen, Vector vista, double n1, double n2, int);

    /**
     * Metodo que ejecuta cada thread.
     */
    int execute_thread(int, int,  int * , std::list<Rayo> , Color * );

    /**
     * Lanza los fotones en la escena y los guarda en un mapa.
     */
    void trazar_fotones();

    /**
     * Lanza los fotones en la escena y los guarda en un mapa.
     */
    void trazar_luces();


};

#endif
