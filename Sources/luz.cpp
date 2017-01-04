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

std::vector<Rayo> Luz::muestrearFotones(int n){
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Vector v;
    std::vector<Rayo> resultado;
    Rayo r;

    for (int i = 0; i < n; ++i)
    {
   		v.set_values(dist(mt),dist(mt),dist(mt));
      v.normalizar();
   		r.set_values(origen, v);
   		resultado.push_back(r);
    }

    return resultado;
}