#include "camara.hpp"


void Camara::set_values(Punto p, Vector v1, Vector v2, Vector v3, int resolucionX,
	int resolucionY, int numPixels){
	distancia = resolucionY / (2 * 0.5773);
	resX = resolucionX;
	resY = resolucionY;
	posicion = p;
	ejes[0] = v1;
	ejes[1] = v2;
	ejes[2] = v3;

	pixels = numPixels;

	plano = sumaPuntoVector(posicion, valorPorVector(v1, distancia));
}

Vector Camara::getVX() { return ejes[0]; }
Vector Camara::getVY() { return ejes[1]; }
Vector Camara::getVZ() { return ejes[2]; }

int Camara::getResX() { return resX; }
int Camara::getResY() { return resY; }
int Camara::getPixels() { return pixels; }

void Camara::setVX(Vector v) { ejes[0] = v; }
void Camara::setVY(Vector v) { ejes[1] = v; }
void Camara::setVZ(Vector v) { ejes[2] = v; }


Punto Camara::getPosicion() { return posicion; }

void Camara::setPosicion(Punto p) { 
	posicion = p; 
	plano = sumaPuntoVector(posicion, valorPorVector(ejes[0], distancia));
}

double Camara::getDistancia(){ return distancia; }


std::list<Rayo> Camara::trazarRayos(){
	int x, y, filas, columnas;
	double areaPixel;
	Punto inicial, arriba, aux;
	std::list<Rayo> rayos;

	//areaPixel =  (resX * resY)*1.0 / (pixels*1.0);
	columnas = sqrt(pixels);
	filas = columnas;
	areaPixel = sqrt(resX*resY*1.0 / (pixels*1.0));

	//Nos colocamos en la esquina superior izquierda.
	arriba = sumaPuntoVector( sumaPuntoVector(plano, valorPorVector(ejes[1], (resY / 2))), valorPorVector(ejes[1], -areaPixel/2));
	inicial = sumaPuntoVector(sumaPuntoVector(arriba, valorPorVector(ejes[2],  -1 * (resX / 2))), valorPorVector(ejes[2], -areaPixel/2));
	aux = inicial;

	//filas = resX / sqrt(areaPixel);
	//columnas = resY / sqrt(areaPixel);

	for ( x = 0; x < filas; x++ ){
		for( y = 0; y < columnas; y++ ){
			
            //Añadir el rayo a la escena.
            Rayo r=trazarRayo(aux);
            rayos.push_back(r);

			aux = sumaPuntoVector(aux, valorPorVector(ejes[2], areaPixel));
		}

		inicial = sumaPuntoVector(inicial, valorPorVector(ejes[1], -areaPixel));
		aux = inicial;
	}

	return rayos;
}

Rayo Camara::trazarRayo(Punto p){
	Rayo r;

	Vector dir = restaPuntos(p, posicion);
	dir.normalizar();
	r.set_values(posicion, dir);
	
	return r;
}

