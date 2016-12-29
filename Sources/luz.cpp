#include "luz.hpp"

Color Luz::getColor(){
	return color;
}

void Luz::atenuar(double distancia){
	//Aguanta la potencia metros al cuadrado sin reducirse
	double ajuste = (potencia * potencia) / (distancia * distancia);
	if ( ajuste < 1.0 ) color.multiplicar(ajuste);
}

Punto Luz::getOrigen(){
	return origen;
}

void Luz::set_values(Punto p, Color c, double pot){
	origen = p;
	color = c;
	potencia = pot;
}

double Luz::getPotencia(){
	return potencia;
}