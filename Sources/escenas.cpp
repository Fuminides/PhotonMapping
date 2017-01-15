/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que renderiza una escena a escoger entre varias.
 */

#include <iostream>

#include "objeto_cartesiano.hpp"
#include "operadorEscena.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include "camara.hpp"
#include "plano.hpp"
#include "triangulo.hpp"
#include "leer_ply.hpp"

#include <math.h>

using namespace std;

int cuentaInt = 1;

int cuenta();

int main(int argc, char ** argv){
    int escenaID = 0;
    operadorEscena escena;
    Camara camara;
    Plano plano;
    Punto origenCamara, origenLuz, origenLuz2, origenLuz3, origenPlano;
    Vector vc1, vc2, vc3, normalPlano;
    Esfera esfera, esfera2, esfera3;
    Color rojo, blanco, azul, negro, verde, naranja, gris, amarillo;
    Luz luz1, luz2, luz3;


    rojo.set_values(255, 0, 0, escena.NORMALIZAR_COLORES);
    blanco.set_values(255, 255, 255, escena.NORMALIZAR_COLORES);
    azul.set_values(0,0, 255, escena.NORMALIZAR_COLORES);
    negro.set_values(0,0,0, escena.NORMALIZAR_COLORES);
    verde.set_values(0,255,0, escena.NORMALIZAR_COLORES);
    naranja.set_values(255,128,0, escena.NORMALIZAR_COLORES);
    gris.set_values(122,122,122, escena.NORMALIZAR_COLORES);
    amarillo.set_values(255,255,0, escena.NORMALIZAR_COLORES);

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    clock_t begin = clock();

    if ( argc > 1 ){
        escenaID = atoi(argv[1]);
    }

    if ( escenaID == 2 ){
        esfera.setOrigen(90, -10, 50);
        esfera.setColor(azul);
        esfera.setRadio(10);
        esfera.setId(cuenta());
        esfera.setReflejo(0.0);
        esfera.setRefraccion(0.2);
        esfera.setCoefRefraccion(0.2);
        esfera.setBRDF(0);

        esfera2.setOrigen(30, 20, -20);
        esfera2.setColor(verde);
        esfera2.setRadio(10);
        esfera2.setId(cuenta());
        esfera2.setReflejo(0.0);
        esfera2.setRefraccion(0.1);
        esfera2.setCoefRefraccion(0.4);
        esfera2.setBRDF(0);

        esfera3.setOrigen(100, 0, -10);
        esfera3.setColor(rojo);
        esfera3.setRadio(20);
        esfera3.setReflejo(0.0);
        esfera3.setId(cuenta());
        esfera3.setRefraccion(0);
        esfera3.setCoefRefraccion(0);
        esfera3.setBRDF(1);

        origenPlano.set_values(200, -150, -150);
        normalPlano.set_values(-1,0,0);
        plano.set_values(origenPlano, normalPlano, vc3, 500, vc2, 500);
        plano.setColor(naranja);
        plano.setReflejo(0.2);
        plano.setRefraccion(0);
        plano.setCoefRefraccion(0);

        origenLuz.set_values(0, 0, 10);
        luz1.set_values(origenLuz, blanco, 30);

        origenLuz2.set_values(65, 10, 10);
        luz2.set_values(origenLuz2, blanco, 30);

        origenLuz3.set_values(100, -50, 0);
        luz3.set_values(origenLuz3, blanco, 80);

        camara.set_values(origenCamara, vc1, vc2, vc3, 10* 100, 10 * 100,  pow(400,2) );

        
        escena.anyadirFigura(&esfera2);
        escena.anyadirFigura(&esfera);
        escena.anyadirFigura(&esfera3);
        escena.anyadirFigura(&plano);

        
        escena.anyadirLuz(luz1);   
        escena.anyadirLuz(luz2);

        escena.setCamara(camara);
    } 
    else if ( escenaID == 1){
       Plano planoAbajo, planoDerecha, planoIzquierda, planoFondo, planoArriba, lampara;
        Esfera esferaEspejo, esferaRefracc;
        Punto oPA, oPD, oPI, pF, pA, puntoLuz, origenC, origenLampara;
        Vector nPA, nPD, nPI, nF, nA;
        Camara camera;
        double boxLenght = 50, lamparaLen = boxLenght/3;
        Luz luzBox;

        escena.PATH_TRACING = false;
        escena.NUMERO_RAYOS_INDIRECTA = 0;

        oPA.set_values(0, 0, 0);
        oPD.set_values(0, 0, boxLenght);
        oPI.set_values(0, 0, 0);
        pF.set_values(boxLenght, 0, 0);
        pA.set_values(0, boxLenght, 0);
        origenLampara.set_values(boxLenght/2 - lamparaLen, boxLenght-2, boxLenght/2 - lamparaLen/2);

        nPA.set_values(0,-1,0);
        nPD.set_values(0,0,-1);
        nPI.set_values(0,0,1);
        nF.set_values(-1,0,0);
        nA.set_values(0,1,0);

        planoAbajo.set_values(oPA, nPA, vc1, boxLenght, vc3, boxLenght);
        planoAbajo.setColor(gris);
        planoAbajo.setReflejo(0.0);
        planoAbajo.setRefraccion(0.0);
        planoAbajo.setCoefRefraccion(0.0);
        planoAbajo.setBRDF(0);

        planoDerecha.set_values(oPD, nPD, vc2, boxLenght, valorPorVector(vc1,1) , boxLenght);
        planoDerecha.setColor(naranja);
        planoDerecha.setReflejo(0.0);
        planoDerecha.setRefraccion(0.0);
        planoDerecha.setCoefRefraccion(0.0);

        planoIzquierda.set_values(oPI, nPI, vc2, boxLenght, vc1 , boxLenght);
        planoIzquierda.setColor(verde);
        planoIzquierda.setReflejo(0.0);
        planoIzquierda.setRefraccion(0.0);
        planoIzquierda.setCoefRefraccion(0.0);

        planoFondo.set_values(pF, nF, vc2, boxLenght, vc3, boxLenght);
        planoFondo.setColor(gris);
        planoFondo.setReflejo(0.0);
        planoFondo.setRefraccion(0.0);
        planoFondo.setCoefRefraccion(0.0);

        planoArriba.set_values(pA, nA, vc1, boxLenght, vc3, boxLenght);
        planoArriba.setColor(gris);
        planoArriba.setReflejo(0.0);
        planoArriba.setRefraccion(0.0);
        planoArriba.setCoefRefraccion(0.0);

        origenC.set_values(-43.30, 25, 25);
        camera.set_values(origenC, vc1, vc2, vc3, boxLenght, boxLenght,  pow(200,2));

        escena.setCamara(camera);

        puntoLuz.set_values(boxLenght *3/4,boxLenght-2,boxLenght/2);
        luz1.set_values(puntoLuz, gris, 50);

        esfera.setOrigen(boxLenght/2, 8, boxLenght/4);
        esfera.setColor(gris);
        esfera.setRadio(8);
        esfera.setReflejo(1.0);
        esfera.setRefraccion(0.0);
        esfera.setCoefRefraccion(0.0);
        esfera.setBRDF(0);

        esfera2.setOrigen(boxLenght/3, 8, boxLenght * 0.75);
        esfera2.setColor(gris);
        esfera2.setRadio(8);
        esfera2.setReflejo(0.0);
        esfera2.setRefraccion(1.53);
        esfera2.setCoefRefraccion(0.7);
        esfera2.setBRDF(0);

        origenLuz3.set_values(boxLenght * 1/5, boxLenght/2, boxLenght/2);
        luz3.set_values(origenLuz3, blanco, boxLenght/2);

        escena.anyadirLuz(luz3);
        escena.anyadirFigura(&esfera);
        escena.anyadirFigura(&esfera2);
        escena.anyadirFigura(&planoArriba);
        escena.anyadirFigura(&planoFondo);
        escena.anyadirFigura(&planoIzquierda);
        escena.anyadirFigura(&planoDerecha);
        escena.anyadirFigura(&planoAbajo);
    }
    else if (escenaID == 0){
        Plano planoAbajo, planoDerecha, planoIzquierda, planoFondo, planoArriba, lampara;
        Esfera esferaEspejo, esferaRefracc;
        Punto oPA, oPD, oPI, pF, pA, puntoLuz, origenC, origenLampara;
        Vector nPA, nPD, nPI, nF, nA;
        Camara camera;
        double boxLenght = 50, lamparaLen = boxLenght *1/3;
        Luz luzBox;

        //escena.NUMERO_RAYOS_INDIRECTA = 416;
        //escena.PATH_TRACING = true;

        oPA.set_values(0, 0, 0);
        oPD.set_values(0, 0, boxLenght);
        oPI.set_values(0, 0, 0);
        pF.set_values(boxLenght, 0, 0);
        pA.set_values(0, boxLenght, 0);
        origenLampara.set_values(boxLenght/2 - lamparaLen, boxLenght-2, boxLenght/2 - lamparaLen/2);

        nPA.set_values(0,-1,0);
        nPD.set_values(0,0,-1);
        nPI.set_values(0,0,1);
        nF.set_values(-1,0,0);
        nA.set_values(0,1,0);

        planoAbajo.set_values(oPA, nPA, vc1, boxLenght, vc3, boxLenght);
        planoAbajo.setColor(gris);
        planoAbajo.setReflejo(0.0);
        planoAbajo.setRefraccion(0.0);
        planoAbajo.setCoefRefraccion(0.0);

        planoDerecha.set_values(oPD, nPD, vc2, boxLenght, valorPorVector(vc1,1) , boxLenght);
        planoDerecha.setColor(naranja);
        planoDerecha.setReflejo(0.0);
        planoDerecha.setRefraccion(0.0);
        planoDerecha.setCoefRefraccion(0.0);

        planoIzquierda.set_values(oPI, nPI, vc2, boxLenght, vc1 , boxLenght);
        planoIzquierda.setColor(verde);
        planoIzquierda.setReflejo(0.0);
        planoIzquierda.setRefraccion(0.0);
        planoIzquierda.setCoefRefraccion(0.0);

        planoFondo.set_values(pF, nF, vc2, boxLenght, vc3, boxLenght);
        planoFondo.setColor(gris);
        planoFondo.setReflejo(0.0);
        planoFondo.setRefraccion(0.0);
        planoFondo.setCoefRefraccion(0.0);

        planoArriba.set_values(pA, nA, vc1, boxLenght, vc3, boxLenght);
        planoArriba.setColor(blanco);
        planoArriba.setReflejo(0.0);
        planoArriba.setRefraccion(0.0);
        planoArriba.setCoefRefraccion(0.0);

        lampara.set_values(origenLampara, nPA, vc1, lamparaLen, vc3, lamparaLen);
        lampara.setReflejo(0.0);
        lampara.setRefraccion(0.0);
        lampara.setCoefRefraccion(0.0);

        puntoLuz.set_values(12.5,40,15.0);
        luzBox.set_values(puntoLuz, blanco, 30);
        lampara.setLuz(luzBox);

        origenC.set_values(-43.30, 25, 25);
        camera.set_values(origenC, vc1, vc2, vc3, boxLenght, boxLenght, pow(100,2));

        escena.setCamara(camera);

        esfera.setOrigen(boxLenght/2, 8, boxLenght/4);
        esfera.setColor(negro);
        esfera.setRadio(8);
        esfera.setReflejo(1.0);
        esfera.setRefraccion(0.0);
        esfera.setCoefRefraccion(0.0);
        esfera.setBRDF(0);

        esfera2.setOrigen(boxLenght/3, 8, boxLenght * 0.75);
        esfera2.setColor(negro);
        esfera2.setRadio(8);
        esfera2.setReflejo(0.0);
        esfera2.setRefraccion(1.53);
        esfera2.setCoefRefraccion(0.7);
        esfera2.setBRDF(0);

        origenLuz3.set_values(boxLenght/2, boxLenght -5, boxLenght/2);
        luz3.set_values(origenLuz3, blanco, 10);

        escena.anyadirFigura(&lampara);
        escena.anyadirFigura(&esfera);
        escena.anyadirFigura(&esfera2);
        escena.anyadirFigura(&planoArriba);
        escena.anyadirFigura(&planoFondo);
        escena.anyadirFigura(&planoIzquierda);
        escena.anyadirFigura(&planoDerecha);
        escena.anyadirFigura(&planoAbajo);
    }
    else if ( escenaID == 3){
        Plano planoAbajo, planoArriba;
        Punto oPA, pLuz, cam, pluz2, pluz3;
        Esfera esferas[5];
        Luz luz, luz2, estrellas[12], luz3;
        int fila = 0, filas = 7, radio = 10;

        escena.NUMERO_RAYOS_INDIRECTA = 0;

        esferas[0].setOrigen(5, radio * 2.5 + 2, -radio - 2);
        esferas[0].setColor(rojo);
        esferas[0].setRadio(radio);
        esferas[0].setReflejo(0.0);
        esferas[0].setRefraccion(0.0);
        esferas[0].setCoefRefraccion(0.0);
        esferas[0].setBRDF(0);

        escena.anyadirFigura(&esferas[0]);

        esferas[1].setOrigen(5, radio * 2.5 + 2, +radio + 2);
        esferas[1].setColor(rojo);
        esferas[1].setRadio(radio);
        esferas[1].setReflejo(0.0);
        esferas[1].setRefraccion(0.0);
        esferas[1].setCoefRefraccion(0.0);
        esferas[1].setBRDF(0);

        escena.anyadirFigura(&esferas[1]);

        for ( int i = 0; i < 12; i++){
            Punto puntoAux;
            puntoAux.set_values(1500, 0, -2300 + i * 250);
            estrellas[i].set_values(puntoAux, amarillo, 1500);

            escena.anyadirLuz(estrellas[i]);
        }

        pluz2.set_values(1900, 100, 0);
        luz2.set_values(pluz2, blanco, 1300);
        pluz3.set_values(-12, 15, 0);
        luz3.set_values(pluz3, amarillo, 20);


        cam.set_values(-12, 15, 0);

        oPA.set_values(-50,0,-500);
        esfera.setOrigen(0,0,0);

        esfera.setColor(negro);
        esfera.setRadio(2000);
        esfera.setReflejo(0.0);
        esfera.setRefraccion(0.0);
        esfera.setCoefRefraccion(0.0);
        esfera.setBRDF(0);

        planoAbajo.set_values(oPA, vc2, vc1, 2000, vc3, 1000);
        planoAbajo.setColor(azul);
        planoAbajo.setReflejo(0.5);
        planoAbajo.setRefraccion(0.0);
        planoAbajo.setCoefRefraccion(0.0);
        planoAbajo.setBRDF(0);

        escena.anyadirLuz(luz2);
        escena.anyadirLuz(luz3);
        escena.anyadirFigura(&planoAbajo);
        escena.anyadirFigura(&esfera);
        
        camara.set_values(cam, vc1, vc2, vc3, 100, 100,  pow(500,2) );
        escena.setCamara(camara);
    }
    else if ( escenaID == 4 ){

        Punto p1,p2,p3,p4,p5,p6;
        Punto p21,p22,p23,p24,p25,p26;
        Punto pl1, pl2, origenLampara;

        escena.NUMERO_RAYOS_INDIRECTA = 0;

        Plano abajo, fondo, lampara;
        Box bound, bound2;
        Vector desplazamiento = vc3;
        desplazamiento.multiplicar(20);

        string fichero = "apple.ply";
        Triangulo pTr[2000], pTr2[2000];

        origenCamara.set_values(-35, 10, 10);

        p1.set_values(-10, -10, -10);
        p2.set_values(-10, 15, -10);
        p3.set_values(10, -10, -10);

        p4.set_values(-10, -10, 10);
        p5.set_values(-10, 15, 10);
        p6.set_values(10, -10, 10);

        p21.set_values(-10, -10, 10);
        p22.set_values(-10, 10, 10);
        p23.set_values(10, -10, 10);

        p24.set_values(-10, -10, 30);
        p25.set_values(-10, 10, 30);
        p26.set_values(10, -10, 30);

        pl1.set_values(-20, 0, -20);
        pl2.set_values(10, -10, -20);

        origenLampara.set_values(-35, 5, 5);

        lampara.set_values(origenLampara, vc1, vc2, 5, vc3, 5);
        lampara.setReflejo(0.0);
        lampara.setRefraccion(0.0);
        lampara.setCoefRefraccion(0.0);

        abajo.set_values(pl1, vc2, vc1, 100, vc3, 100);
        abajo.setColor(gris);
        abajo.setReflejo(0.0);
        abajo.setRefraccion(0.0);
        abajo.setCoefRefraccion(0.0);
        abajo.setBRDF(0);

        fondo.set_values(pl2, vc1, vc2, 100, vc3, 200);
        fondo.setColor(negro);
        fondo.setReflejo(0.0);
        fondo.setRefraccion(0.0);
        fondo.setCoefRefraccion(0.0);
        fondo.setBRDF(0);

        leer(fichero, pTr);
        leer(fichero, pTr2);

        bound.set_values(p1,p2,p3,p4,p5,p6,nPoligonos());
        bound2.set_values(p21,p22,p23,p24,p25,p26,nPoligonos());

        for (int i = 0; i<nPoligonos(); i++)
        {
            pTr[i].setColor(rojo);
            pTr[i].multiplicar(100);
            bound.meter(&(pTr[i]));

            pTr2[i].setColor(azul);
            pTr2[i].multiplicar(100);
            pTr2[i].desplazar(desplazamiento);
            pTr2[i].setBRDF(1);
            bound2.meter(&(pTr2[i]));            
        }

        escena.anyadirFigura(&bound);
        escena.anyadirFigura(&bound2);
        escena.anyadirFigura(&abajo);
        escena.anyadirFigura(&fondo);

        origenLuz3.set_values(-15, 30, 10);

        luz3.set_values(origenLuz3, blanco, 10);
        lampara.setLuz(luz3);
        escena.anyadirFigura(&lampara);
        camara.set_values(origenCamara, vc1, vc2, vc3, 300, 300,  pow(500,2) );

        escena.PATH_TRACING = true;
        escena.NUMERO_RAYOS_INDIRECTA = 32;

        escena.setCamara(camara);
        escena.dibujar();

        bound.free();
        bound2.free();

        exit(1);
    }

    escena.dibujar();

    clock_t end = clock();
    int elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    double horas = elapsed_secs /3600;
    double minutos = (elapsed_secs%3600) / 60;
    double segundos = (elapsed_secs%3600) % 60;
    cout << "Escena cargada y renderizada en " << horas<< " hora(s), " <<minutos << " minuto(s) y " <<segundos << " segundo(s) de CPU\n"; 
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC / std::thread::hardware_concurrency();
    horas = elapsed_secs /3600;
    minutos = (elapsed_secs%3600) / 60;
    segundos = (elapsed_secs%3600) % 60;
    cout << "Tiempo real estimado: " << horas<< " hora(s), " <<minutos << " minuto(s) y " <<segundos << " segundo(s)\n"; 

}

/* Devuelve enteros consecutivos. */
int cuenta(){ 
    int aux = cuentaInt; //Lo usamos para asignar IDs para las figuras (Debug)
    cuentaInt++;
    return aux;
}