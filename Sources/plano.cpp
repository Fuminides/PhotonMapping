#include "plano.hpp"

void Plano::set_values(Punto o, Vector n, Vector _vy, double alt, Vector _vx, double anch){
	origen = o;
	vNormal = n;
	vNormal.normalizar();
	altura = alt;
	anchura = anch;
	vectorX = _vx;
	vectorX.normalizar();
	vectorY = _vy;
	vectorY.normalizar();

	double a = vNormal.getX(), b = vNormal.getY(), c = vNormal.getZ(),
	px = origen.getX(), py = origen.getY(), pz = origen.getZ();

	d = a*px + b*py + c*pz;

}

double Plano::intersectar(Rayo r){
	double a = vNormal.getX(), b = vNormal.getY(), c = vNormal.getZ(),
	ox = r.getOrigen().getX(), oy = r.getOrigen().getY(), oz = r.getOrigen().getZ(),
	vx = r.getVector().getX(), vy = r.getVector().getY(), vz = r.getVector().getZ(), t;
	Punto aux;
	Vector proyectar;
	t = -(a*ox+b*oy + c*oz-d)/(a*vx+b*vy+c*vz);

	if (  (t*t < 1) | (t < 0)){
		return -1;
	}

	vx = vx * t + ox;
	vy = vy * t + oy;
	vz = vz * t + oz;

	aux.set_values(vx, vy,vz);
	proyectar = restaPuntos(aux, origen);
	//std::cout << "Prod escal: " << std::to_string(productoEscalar(proyectar, vectorX)) << "\n";
	if ( (productoEscalar(proyectar, vectorX) > anchura) | (productoEscalar(proyectar, vectorX) < 0)) {
		t = -1;
	}
	else if (( productoEscalar(proyectar, vectorY) > altura ) | (productoEscalar(proyectar, vectorY) < 0)){
		t = -1;
	}

	return t;
}

Vector Plano::normal(Punto p){
	return vNormal;
}

void Plano::setLuz(Luz _l){
	double intensidad = _l.getPotencia();
	Color cLuz = _l.getColor();
	Punto aux;
	color = cLuz;
	luz = true;

	luces[0].set_values(origen, cLuz, intensidad);
	aux = sumaPuntoVector(origen, valorPorVector(vectorX, anchura));
	luces[1].set_values(aux, cLuz, intensidad);
	aux = sumaPuntoVector(aux, valorPorVector(vectorY, altura));
	luces[2].set_values(aux, cLuz, intensidad);
	aux = sumaPuntoVector(origen, valorPorVector(vectorY, altura));
	luces[3].set_values(aux, cLuz, intensidad);
	aux = sumaPuntoVector(origen, valorPorVector(vectorY, altura/2));
	aux = sumaPuntoVector(aux, valorPorVector(vectorX, anchura/2));
	luces[4].set_values(aux, cLuz, intensidad);

}

std::vector<Luz> Plano::getLuces(){
	std::vector<Luz> v(std::begin(luces), std::end(luces));    
    return v;
}