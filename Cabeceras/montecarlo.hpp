/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que facilita el sampling para resolver la ecuación de render por montecarlo.
 */
#ifndef MONTECARLO_H
#define MONTECARLO_H
#include "objeto_cartesiano.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"

#include <list>
#include <random>

using namespace std;
class Montecarlo
{
    double inclination[1500];
    double azimuth[1500];
    int num;
    Vector x;
    Vector n;
    
public:
    void set_values(Vector x, Vector n,int numRayos);
    
    double getInclination(int i);
    double getAzimuth(int i);
    Vector getn();
    Vector getx();
    int getNum();
    
    Matriz calcularT();
    double determinante(Matriz T);
    Matriz inversaT(Matriz T);
        
    list<Vector> calcularw();
    void numAleatorios();
    void cNegativa();
    Vector multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3);
    
    void mostrarA();
    void mostrarI();
};

#endif // MONTECARLO_H
