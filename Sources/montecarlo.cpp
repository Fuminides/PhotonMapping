/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a montecarlo y cosine sampling 
 */
#include <iostream>
#include <cmath>
#include "montecarlo.hpp"
#include "objeto_cartesiano.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include <vector>
#include <stdlib.h> 
#include <algorithm>
#include <math.h>
using namespace std;
#define PI 3.14159265

void Montecarlo::set_values(Vector px, Vector normal, int numRayos)
{
    n = normal;
    x = px;
    x.normalizar();
    num = numRayos;
}
double Montecarlo::getInclination(int i){
    return inclination[i];
}
double Montecarlo::getAzimuth(int i){
    return azimuth[i];
}
Vector Montecarlo::getn(){
    return n;
}
Vector Montecarlo::getx(){
    return x;
}
int Montecarlo::getNum(){
    return num;
}
/*
 *Calcula N matrices T 4x4 
 */
Matriz Montecarlo::calcularT(){
    
    Vector azar;
    Matriz matT;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);


    azar.set_values(dist(mt),dist(mt),dist(mt));

    while((azar.getX()==0 && azar.getY()==0 && azar.getZ()==0)){
        azar.set_values(dist(mt),dist(mt),dist(mt));
    }
 
    Vector n= getn();
    Vector x = getx();
    
    Vector u = productoVectorial(azar,n);
    u.normalizar();
    Vector v = productoVectorial(n,u);
    v.normalizar();
    
   
    matT.set_valuesColum(u,v,n,x);
     

    return matT;
}
/*
 * Devuelve el determinante de la matriz 4x4 T 
 */
double Montecarlo::determinante(Matriz T){
    double t1 = T.get(2,1)*T.get(3,2)*T.get(4,4) + T.get(3,1)*T.get(4,2)*T.get(2,4) + T.get(2,2)*T.get(3,4)*T.get(4,1) - T.get(4,1)*T.get(3,2)*T.get(2,4) - T.get(4,2)*T.get(3,4)*T.get(2,1) - T.get(3,1)*T.get(2,2)*T.get(4,4);
    double t2 = T.get(1,1)*T.get(3,2)*T.get(4,4) + T.get(1,2)*T.get(3,4)*T.get(4,1) + T.get(3,1)*T.get(4,2)*T.get(1,4) - T.get(4,1)*T.get(3,2)*T.get(1,4) - T.get(4,2)*T.get(3,4)*T.get(1,1) - T.get(1,2)*T.get(3,1)*T.get(4,4);
    double t3 = T.get(1,1)*T.get(2,2)*T.get(4,4) + T.get(2,1)*T.get(4,2)*T.get(1,4) + T.get(1,2)*T.get(2,4)*T.get(4,1) - T.get(4,1)*T.get(2,2)*T.get(1,4) - T.get(4,2)*T.get(2,4)*T.get(1,1) - T.get(2,1)*T.get(1,2)*T.get(4,4);
    double t4 = T.get(1,1)*T.get(2,2)*T.get(3,4) + T.get(2,1)*T.get(3,2)*T.get(1,4) + T.get(1,2)*T.get(2,4)*T.get(3,1) - T.get(3,1)*T.get(2,2)*T.get(1,4) - T.get(3,2)*T.get(2,4)*T.get(1,1) - T.get(2,1)*T.get(1,2)*T.get(3,4); 
    
    return (T.get(1,3)*t1 - T.get(2,3)*t2 + T.get(3,3)*t3 - T.get(4,3)*t4);
    
}
/*
 * Calcula la inversa de la matriz T 4x4
 */
Matriz Montecarlo::inversaT(Matriz T){
    double det = determinante(T);
    
    Matriz nueva;
    Vector u;
    
    u.set_X((T.get(2,2)*T.get(3,3)*T.get(4,4) + T.get(3,2)*T.get(4,3)*T.get(2,4) + T.get(4,2)*T.get(2,3) *T.get(3,4) -T.get(4,2)*T.get(3,3)*T.get(2,4) - T.get(3,2)*T.get(2,3)*T.get(4,4) - T.get(2,2)*T.get(4,3)*T.get(3,4))/det);
    u.set_Y(-(T.get(2,1)*T.get(3,3)*T.get(4,4) + T.get(3,1)*T.get(4,3)*T.get(2,4) + T.get(4,1)*T.get(2,3) *T.get(3,4) -T.get(4,1)*T.get(3,3)*T.get(2,4) - T.get(3,1)*T.get(2,3)*T.get(4,4) - T.get(2,1)*T.get(4,3)*T.get(3,4))/det);
    u.set_Z((T.get(2,1)*T.get(3,2)*T.get(4,4) + T.get(3,1)*T.get(4,2)*T.get(2,4) + T.get(4,1)*T.get(2,2) *T.get(3,4) -T.get(4,1)*T.get(3,2)*T.get(2,4) - T.get(3,1)*T.get(2,2)*T.get(4,4) - T.get(2,1)*T.get(4,2)*T.get(3,4))/det);
    u.set_D(-(T.get(2,1)*T.get(3,2)*T.get(4,3) + T.get(3,1)*T.get(4,2)*T.get(2,3) + T.get(4,1)*T.get(2,2) *T.get(3,3) -T.get(4,1)*T.get(3,2)*T.get(2,3) - T.get(3,1)*T.get(2,2)*T.get(4,3) - T.get(2,1)*T.get(4,2)*T.get(3,3))/det);
    nueva.setVector1(u);
    
    u.set_X(-(T.get(1,2)*T.get(3,3)*T.get(4,4) + T.get(4,2)*T.get(1,3)*T.get(3,4) + T.get(3,2)*T.get(4,3) *T.get(1,4) -T.get(4,2)*T.get(3,3)*T.get(1,4) - T.get(3,2)*T.get(1,3)*T.get(4,4) - T.get(1,2)*T.get(4,3)*T.get(3,4))/det);
    u.set_Y((T.get(1,1)*T.get(3,3)*T.get(4,4) + T.get(3,1)*T.get(4,3)*T.get(1,4) + T.get(4,1)*T.get(1,3) *T.get(3,4) -T.get(4,1)*T.get(3,3)*T.get(1,4) - T.get(3,1)*T.get(1,3)*T.get(4,4) - T.get(1,1)*T.get(4,3)*T.get(3,4))/det);    
    u.set_Z(-(T.get(1,1)*T.get(3,2)*T.get(4,4) + T.get(3,1)*T.get(4,2)*T.get(1,4) + T.get(4,1)*T.get(1,2) *T.get(3,4) -T.get(4,1)*T.get(3,2)*T.get(1,4) - T.get(3,1)*T.get(1,2)*T.get(4,4) - T.get(1,1)*T.get(4,2)*T.get(3,4))/det);    
    u.set_D((T.get(1,1)*T.get(3,2)*T.get(4,3) + T.get(3,1)*T.get(4,2)*T.get(1,3) + T.get(4,1)*T.get(1,2) *T.get(3,3) -T.get(4,1)*T.get(3,2)*T.get(1,3) - T.get(3,1)*T.get(1,2)*T.get(4,3) - T.get(1,1)*T.get(4,2)*T.get(3,3))/det);
    nueva.setVector2(u);
    
    u.set_X((T.get(1,2)*T.get(2,3)*T.get(4,4) + T.get(2,2)*T.get(4,3)*T.get(1,4) + T.get(4,2)*T.get(1,3) *T.get(2,4) -T.get(2,2)*T.get(1,3)*T.get(4,4) - T.get(1,2)*T.get(4,3)*T.get(2,4) - T.get(4,2)*T.get(2,3)*T.get(1,4))/det);    
    u.set_Y(-(T.get(1,1)*T.get(2,3)*T.get(4,4) + T.get(2,1)*T.get(4,3)*T.get(1,4) + T.get(4,1)*T.get(1,3) *T.get(2,4) -T.get(4,1)*T.get(2,3)*T.get(1,4) - T.get(2,1)*T.get(1,3)*T.get(4,4) - T.get(1,1)*T.get(4,3)*T.get(2,4))/det);    
    u.set_Z((T.get(1,1)*T.get(2,2)*T.get(4,4) + T.get(2,1)*T.get(4,2)*T.get(1,4) + T.get(4,1)*T.get(1,2) *T.get(2,4) -T.get(4,1)*T.get(2,2)*T.get(1,4) - T.get(2,1)*T.get(1,2)*T.get(4,4) - T.get(1,1)*T.get(4,2)*T.get(2,4))/det);   
    u.set_D(-(T.get(1,1)*T.get(2,2)*T.get(4,3) + T.get(2,1)*T.get(4,2)*T.get(1,3) + T.get(4,1)*T.get(1,2) *T.get(2,3) -T.get(4,1)*T.get(2,2)*T.get(1,3) - T.get(2,1)*T.get(1,2)*T.get(4,3) - T.get(1,1)*T.get(4,2)*T.get(2,3))/det);
    nueva.setVector3(u);
    
    u.set_X(-(T.get(1,2)*T.get(2,3)*T.get(3,4) + T.get(2,2)*T.get(3,3)*T.get(1,4) + T.get(3,2)*T.get(1,3) *T.get(2,4) -T.get(3,2)*T.get(2,3)*T.get(1,4) - T.get(2,2)*T.get(1,3)*T.get(3,4) - T.get(1,2)*T.get(3,3)*T.get(2,4))/det);    
    u.set_Y((T.get(1,1)*T.get(2,3)*T.get(3,4) + T.get(2,1)*T.get(3,3)*T.get(1,4) + T.get(3,1)*T.get(1,3) *T.get(2,4) -T.get(3,1)*T.get(2,3)*T.get(1,4) - T.get(2,1)*T.get(1,3)*T.get(3,4) - T.get(1,1)*T.get(3,3)*T.get(2,4))/det);    
    u.set_Z(-(T.get(1,1)*T.get(2,2)*T.get(3,4) + T.get(2,1)*T.get(3,2)*T.get(1,4) + T.get(3,1)*T.get(1,2) *T.get(2,4) -T.get(3,1)*T.get(2,2)*T.get(1,4) - T.get(2,1)*T.get(1,2)*T.get(3,4) - T.get(1,1)*T.get(3,2)*T.get(2,4))/det);  
    u.set_D((T.get(1,1)*T.get(2,2)*T.get(3,3) + T.get(2,1)*T.get(3,2)*T.get(1,3) + T.get(3,1)*T.get(1,2) *T.get(2,3) -T.get(3,1)*T.get(2,2)*T.get(1,3) - T.get(2,1)*T.get(1,2)*T.get(3,3) - T.get(1,1)*T.get(3,2)*T.get(2,3))/det);
    nueva.setVector4(u);
    
    Vector tt1 = nueva.getVector(1);
    Vector tt2 = nueva.getVector(2);
    Vector tt3 = nueva.getVector(3);
    Vector tt4 = nueva.getVector(4);
    
    Vector n;
    n.set_values(tt1.get(1),tt2.get(1),tt3.get(1),tt4.get(1));
    nueva.setVector1(n);
    
    n.set_values(tt1.get(2),tt2.get(2),tt3.get(2),tt4.get(2));
    nueva.setVector2(n);
    
    n.set_values(tt1.get(3),tt2.get(3),tt3.get(3),tt4.get(3));
    nueva.setVector3(n);
    
    n.set_values(tt1.get(4),tt2.get(4),tt3.get(4),tt4.get(4));
    nueva.setVector4(n);
    
   
    return nueva;    
}
/*
 * Transforma el vector dado de coordenadas locales a coordenadas globales
 */
Vector Montecarlo::multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3){
    Vector w;
    
    w.set_X(T1.get(1,1)*x1+T1.get(1,2)*x2+T1.get(1,3)*x3);
    w.set_Y(T1.get(2,1)*x1+T1.get(2,2)*x2+T1.get(2,3)*x3);
    w.set_Z(T1.get(3,1)*x1+T1.get(3,2)*x2+T1.get(3,3)*x3);
    w.set_D(T1.get(4,1)*x1+T1.get(4,2)*x2+T1.get(4,3)*x3);
    
    return w;
}
/*
 * Calcula N vectores en coordenadas globales
 * Devuelve un vector de Vectores para poder aplicarles despues la ecuacion de render.
 */
list<Vector> Montecarlo::calcularw(){
    Matriz matT= calcularT();
    Matriz T1 = inversaT(matT);
    list<Vector> vect;
    //inicializa los inclination y azimuth 
    numAleatorios();
    cNegativa();
    int n = getNum();
    
    for(int i=0;i<n;i++){
        Vector wi = multiplicarMatrizValores(T1,sin(getInclination(i))*cos(getAzimuth(i)),sin(getInclination(i))*sin(getAzimuth(i)),cos(getInclination(i)));
        vect.push_back(wi);
    }
   return vect;
}
/*
 * Calcula N numeros aleatorios entre [0,1) para utilizar cosine sampling 
 * ( tantos numeros como vectores se han generado en montecarlo)
 * Devuelve un vector con todos los numeros aleatorios generados para poder 
 * aplicarles la C^(-1) posteriormente.
 */
void Montecarlo::numAleatorios(){
    double x=0.0;
    int N =getNum();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for(int i=0;i<N;i++){
         x = (dist(mt));
         inclination[i]=x;
    }
}
/*
 * Calcula la C^(-1) aplicando cosine sampling a cada uno de los numeros generados
 * aleatoriamente . 
 * Devuelve un vector con todas las C^(-1) para poder aplicarlas en la ecuacion de render
 * posteriormente
 */
void Montecarlo::cNegativa(){
    double x=0.0,cn=0.0,param=0.0;
    int N =getNum();
    for(int i=0;i<N;i++){
         x=getInclination(i);
         param = sqrt(1-x);
         cn = acos (param)* 180.0 / PI;
         azimuth[i]=cn;
    }
}

void Montecarlo::mostrarA(){
    int N =getNum();
    for(int i=0;i<N;i++){
         cout<<"dato pos "<<i<<" : "<<azimuth[i]<<endl;
    }
}


void Montecarlo::mostrarI(){
     int N = getNum();
    for(int i=0;i<N;i++){
         cout<<"dato pos "<<i<<" : "<<inclination[i]<<endl;
    }
}