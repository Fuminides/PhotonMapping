/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a esfera, con un origen y un radio. (Y las demas propiedades de las figuras.)
 */
#ifndef esfera_h_
#define esfera_h_

#include <math.h>
#include "punto.hpp"
#include "vector.hpp"
#include "color.hpp"
#include "figura.hpp"
#include "operadorVector.hpp"

#include <vector>


 class Esfera : public Figura{

        double radio;
        Punto origen;
        Color color;
        Luz luces[6];
        const double UMBRAL = 1.000000;

        double puntoPorPunto(Punto p1, Punto p2);
        double puntoPorVector(Punto p1, Vector p2);

     public:
        
        /*
         * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
         * la intercepte
         */
        double intersectar(Rayo);

        /*
         * Devuelve el radio de la esfera
         */ 
        double getRadio();
        /*
         * Asigna un valor al radio 
         */
        void setRadio(double r);
        /*
         * Devuelve la superficie de la esfera
         */
        double superficie();
        /*
         * Devuelve el origen de la esfera en el plano
         */
        Punto getOrigen();
        /*
         * Asigna el valor del origen de la esfera
         */
        void setOrigen(double x, double y,double z);
        /*
         * Devuelve la normal de la figura dado un punto
         */
         Vector normal(Punto);

        /*
         * Devuelve falso
         */
         bool isBox() { return false; }

         //---LUCES DE AREA-----
         /*
          * Devuelve el conjunto de luces, en caso de ser una luz de area.
          */
         std::vector<Luz> getLuces();

         /*
          * Añade una luz para conseguir una luz de area.
          */
         void setLuz(Luz);

         //TODO
         std::vector<Rayo> muestrearFotones(int n){;}

 };

 #endif