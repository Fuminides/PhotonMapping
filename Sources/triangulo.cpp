#include "triangulo.hpp"

void Triangulo::set_values(Punto p, Punto pp, Punto ppp){
	p1 = p;
	p2 = pp;
	p3 = ppp;

	v1 = restaPuntos(p2,p1);
	v2 = restaPuntos(p3,p1);
	v3 = restaPuntos(p3,p2);
}

Vector Triangulo::normal(Punto p){
	Vector aux1, aux2, result;
	Punto pivote1, pivote2;

	pivote1 = p1;
	pivote2 = p2;
	if ( p.equals(p1)){
		pivote1 = p3;
	}
	else if ( p.equals(p2)){
		pivote2 = p3;
	}

	aux1 = restaPuntos(pivote1,p);
	aux2 = restaPuntos(pivote2,p);

	result = productoVectorial(aux1, aux2);
	result.normalizar();

	return result;
}

void Triangulo::setLuz(Luz l){
	luz = true;
	Punto oLuz;

	oLuz = sumaPuntoVector(p1, valorPorVector(v1, 0.5));
	oLuz = sumaPuntoVector(oLuz, valorPorVector(v2, 0.5));

	foco.set_values(oLuz, l.getColor(), l.getPotencia());
	color = l.getColor();
}

std::vector<Luz> Triangulo::getLuces(){
	std::vector<Luz> v;
	v.push_back(foco);
	return v;
}

double Triangulo::intersectar(Rayo r)
{
		Punto O = r.getOrigen();
		Vector D = r.getVector();

  Vector e1, e2;  //Edge1, Edge2
  Vector P, Q, T;
  float det, inv_det, u, v;
  float t;

  //Find vectors for two edges sharing p1
  e1 = restaPuntos(p2, p1);
  e2 = restaPuntos(p3, p1);
  //Begin calculating determinant - also used to calculate u parameter
  P = productoVectorial(D, e2);
  //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
  det = productoEscalar(e1, P);
  //NOT CULLING
  if(det > -EPSILON && det < EPSILON) return -1;
  inv_det = 1.f / det;

  //calculate distance from p1 to ray origin
  T = restaPuntos(O, p1);

  //Calculate u parameter and test bound
  u = productoEscalar(T, P) * inv_det;
  //The intersection lies outside of the triangle
  if(u < 0.f || u > 1.f) return -1;

  //Prepare to test v parameter
  Q = productoVectorial(T, e1);

  //Calculate V parameter and test bound
  v = productoEscalar(D, Q) * inv_det;
  //The intersection lies outside of the triangle
  if(v < 0.f || u + v  > 1.f) return -1;

  t = productoEscalar(e2, Q) * inv_det;

  if(t > EPSILON) { //ray intersection
    return t;
  }
  // No hit, no win
  return -1;
}

std::string Triangulo::to_string(){
	std::string s = "";
	s += "Vertice 1: " + p1.to_string() + "\n";
	s += "Vertice 2: " + p2.to_string()+ "\n";
	s += "Vertice 3: " + p3.to_string()+ "\n"; 

	return s;
}

void Triangulo::multiplicar(double k){
	p1.multiplicar(k);
	p2.multiplicar(k);
	p3.multiplicar(k);

	v1 = restaPuntos(p2,p1);
	v2 = restaPuntos(p3,p1);
	v3 = restaPuntos(p3,p2);	
}

void Triangulo::desplazar(Vector v){
	p1 = sumaPuntoVector(p1, v);
	p2 = sumaPuntoVector(p2, v);
	p3 = sumaPuntoVector(p3, v);

	v1 = restaPuntos(p2,p1);
	v2 = restaPuntos(p3,p1);
	v3 = restaPuntos(p3,p2);	
}