#include "operadorVector.hpp"

double multiplicacionMatricial(Vector v1, Vector v2){
	//Solo para vectores 3D!!
	return v1.getX()*v2.getX() + v1.getY()*v2.getY() + v1.getZ()*v2.getZ();
}

double productoEscalar(Vector v1, Vector v2){
	return multiplicacionMatricial(v1, v2);
}

Vector productoVectorial(Vector v1, Vector v2){
	double x1 = v1.getX(), x2 = v2.getX(),
		y1 = v1.getY(), y2 = v2.getY(),
		z1 = v1.getZ(), z2 = v2.getZ(), xR, yR, zR;
	Vector vR;

	xR = y1 * z2 - z1 * y2;
	yR = x1 * z2 - x2 * z1;
	zR = x1 * y2 - x2 * y1;
	vR.set_values(xR, -yR, zR);

	return vR; 
}

Vector sumaVectores(Vector v1,Vector v2){
	Vector vR;
    vR.set_values(v1.getX()+v2.getX(),v1.getY()+v2.getY(),v1.getZ()+v2.getZ());
    return vR;
}

Vector restaVectores(Vector v1,Vector v2){
	Vector vR;
    vR.set_values(v1.getX()-v2.getX(),v1.getY()-v2.getY(),v1.getZ()-v2.getZ());
    return vR;
}

Vector restaPuntos(Punto p1,Punto p2){
	Vector vR;
    vR.set_values(p1.getX()-p2.getX(), p1.getY()-p2.getY(), p1.getZ()-p2.getZ());
    return vR;
}

Vector valorPorVector(Vector v,double alfa){
	Vector vR;
    vR.set_values(alfa*v.getX(),alfa*v.getY(),alfa*v.getZ());
    return vR;
}

Punto sumaPuntoVector(Punto p,Vector v){
	Punto p2;
    p2.set_values(p.getX()+v.getX(),p.getY()+v.getY(),p.getZ()+v.getZ());
    return p2;
}

Punto sumaPuntos(Punto p, Punto p2){
	Punto pR;
    pR.set_values(p.getX()+p2.getX(),p.getY()+p2.getY(),p.getZ()+p2.getZ());
    return pR;
}

Vector matrizPorVector(Matriz m, Vector v){
	Vector vR;
	double a1 = m.get(1,1), a2 = m.get(2,1), a3 = m.get(3,1), a4 = m.get(4,1),
		b1 = m.get(1,2), b2 = m.get(2,2), b3 = m.get(3,2), b4 = m.get(4,2),
		c1 = m.get(1,3), c2 = m.get(2,3), c3 = m.get(3,3), c4 = m.get(4,3),
		v1 = v.getX(), v2 = v.getY(), v3 = v.getZ(), v4 = v.getD(),
		vR1, vR2, vR3, VR4;

	vR1 = v1 * a1 + v2 * a2 + v3 * a3 + v4 * a4;
	vR2 = v1 * b1 + v2 * b2 + v3 * b3 + v4 * a4;
	vR3 = v1 * c1 + v2 * c2 + v3 * c3 + v4 * c4;

	vR.set_values(vR1, vR2, vR3);

	return vR;
}

Punto matrizPorPunto(Matriz m, Punto v){
	Punto vR;
	double a1 = m.get(1,1), a2 = m.get(2,1), a3 = m.get(3,1), a4 = m.get(4,1),
		b1 = m.get(1,2), b2 = m.get(2,2), b3 = m.get(3,2), b4 = m.get(4,2),
		c1 = m.get(1,3), c2 = m.get(2,3), c3 = m.get(3,3), c4 = m.get(4,3),
		v1 = v.getX(), v2 = v.getY(), v3 = v.getZ(), v4 = v.getD(),
		vR1, vR2, vR3, VR4;

	vR1 = v1 * a1 + v2 * a2 + v3 * a3 + v4 * a4;
	vR2 = v1 * b1 + v2 * b2 + v3 * b3 + v4 * a4;
	vR3 = v1 * c1 + v2 * c2 + v3 * c3 + v4 * c4;

	double d1 = m.get(1,4), d2 = m.get(2,4), d3 = m.get(3,4), d4 = m.get(4,4);
	double vR4 = v1 * d1 + v2 * d2 + v3 * d3 + v4 * d4;
	vR.set_values(vR1 / vR4, vR2 / vR4, vR3 / vR4);

	return vR;
}