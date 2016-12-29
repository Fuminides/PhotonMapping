/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a cualquier elemento con x, y, z. Es decir, que
 * este definido por tres coordenadas cartesianas.
 */
#include "objeto_cartesiano.hpp"

double ObjetoCartesiano::getX() {return valores[0];}
double ObjetoCartesiano::getY() {return valores[1];}
double ObjetoCartesiano::getZ() {return valores[2];}
double ObjetoCartesiano::getD() {return valores[3];}



void ObjetoCartesiano::set_values(double _x, double _y, double _z){
	valores[0] = _x;
	valores[1] = _y;
	valores[2] = _z;
	if ( esVector() ){
		valores[3] = 0;
	}
	else{
		valores[3] = 1;
	}

}

void ObjetoCartesiano::set_values(double _x, double _y, double _z, double _d){
	valores[0] = _x;
	valores[1] = _y;
	valores[2] = _z;
	valores[3] = _d;
}

void ObjetoCartesiano::set_X(double valor){
	valores[0] = valor;
}

void ObjetoCartesiano::set_Y(double valor){
	valores[1] = valor;
}

void ObjetoCartesiano::set_Z(double valor){
	valores[2] = valor;
}

void ObjetoCartesiano::set_D(double valor){
	valores[3] = valor;
}

double ObjetoCartesiano::get(int indice){
	return valores[indice-1];
}

std::string ObjetoCartesiano::to_string(){
	return "X: " + std::to_string(valores[0]) +  ", Y: " +  std::to_string(valores[1]) + ", Z:" + std::to_string(valores[2]);
}

void ObjetoCartesiano::multiplicar(double k){
	set_X(getX()*k);
	set_Y(getY()*k);
	set_Z(getZ()*k);
}