#include "color.hpp"

void Color::set_values(int _rojo, int _verde, int _azul, bool n){
	red = _rojo*1.0 / (RANGO * 1.0);
	green = _verde*1.0 / (RANGO * 1.0);
	blue = _azul*1.0 / (RANGO * 1.0);

	rojo = _rojo;
	verde = _verde;
	azul = _azul;

	normaliza = n;

	centinela = true;
}

void Color::set_values_acum(int _rojo, int _verde, int _azul){
	red += _rojo*1.0 / (RANGO * 1.0);
	green += _verde*1.0 / (RANGO * 1.0);
	blue += _azul*1.0 / (RANGO * 1.0);

	rojo += _rojo;
	verde += _verde;
	azul += _azul;

	centinela = true;
}

unsigned char Color::splashR(){
	if (!normaliza){
		if (red*RANGO > 245.5) return 255;
		return  (unsigned char) (red * RANGO);	
	} 
	unsigned char aux = (unsigned char) (((int) rojo) % 255);
	if ( rojo >= 254.5 ) aux = 255;
	return aux;
}
unsigned char Color::splashG(){
	if (!normaliza){
		if (green*RANGO > 245.5) return 255;
		return  (unsigned char) (green * RANGO);	
	}
	unsigned char aux = (unsigned char) (((int) verde) % 255);
	if ( verde >= 254.5 ) aux = 255;
	return aux;
}
unsigned char Color::splashB(){
	if (!normaliza){
		if (blue*RANGO > 245.5) return 255;
		return  (unsigned char) (blue * RANGO);	
	}
	unsigned char aux = (unsigned char) (((int) azul) % 255);
	if ( azul >= 254.5 ) aux = 255;
	return aux;
}

void Color::multiplicar(double k){
	if (!normaliza){
		if ( red * k > 1.0 ) red = 1.0;
		else{
			red = red * k;
		} 
		
		if ( green * k > 1.0 ) green = 1.0;
		else{
			green = green * k;
		} 

		if ( blue * k > 1.0 ) blue = 1.0;
		else{
			blue = blue * k;
		}
	}
	else{
		rojo = rojo * k;
		verde = verde * k;
		azul = azul * k;
	}

}

void Color::sumar(Color c){
	double cred = c.red;
	double cgreen = c.green;
	double cblue = c.blue;
	
	if (!normaliza){
		if ( red + cred > 1.0) { red = 1.0; }
		else if ( red + cred < 0.0 ) { red = 0.0; }
		else {red += (cred); }

		if ( green + cgreen > 1.0) { green= 1.0; }
		else if ( green + cgreen < 0.0 ) { green = 0.0; }
		else {green += (cgreen);}

		if ( blue + cblue > 1.0) { blue = 1.0; }
		else if ( blue + cblue < 0.0 ) { blue = 0.0; }
		else {blue += (cblue);}
	}
	else{
		rojo += c.rojo;
		verde += c.verde;
		azul += c.azul;
	}
}

std::string Color::to_string(){
	if (!normaliza) return "R: " + std::to_string(splashR()) + ", G: " + std::to_string(splashG()) + ", B: " + std::to_string(splashB());

	return "R: " + std::to_string((unsigned char) (((int) rojo) % 255)) + ", G: " + std::to_string((unsigned char) (((int) verde) % 255)) + ", B: " + std::to_string((unsigned char) (((int) azul) % 255))
	+ ", Originales: R: " + std::to_string(rojo) + ", G: " + std::to_string(verde) + ", B: " + std::to_string(azul);
}

void Color::normalizar(double max){
	if ( normaliza ){
		rojo = rojo / max * RANGO;
		azul = azul / max * RANGO;
		verde = verde / max * RANGO;
	}
}

double Color::max(){
	if ( normaliza ){
		if ( (rojo >= green) && (rojo >= azul) ) return rojo;
		if ( (azul >= verde) && (azul >= rojo) ) return azul;
		if ( (verde >= rojo) && (verde >= azul) ) return verde;
	}
	else{
		if ( (red >= green) && (red >= blue) ) return red;
		if ( (blue >= green) && (blue >= red) ) return blue;
		if ( (green >= red) && (green >= blue) ) return green;	
	}
}