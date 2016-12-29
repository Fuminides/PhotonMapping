#include "matriz.hpp"

void Matriz::set_values(Vector _v1, Vector _v2, Vector _v3){
	Vector vR;
	vR.set_values(0.0,0.0,0.0,1.0);

	vectores[0] = _v1;
	vectores[1] = _v2;
	vectores[2] = _v3;
	vectores[3] = vR;
}

void Matriz::set_values(Vector _v1, Vector _v2, Vector _v3, Vector _v4){
	vectores[0] = _v1;
	vectores[1] = _v2;
	vectores[2] = _v3;
	vectores[3] = _v4;
}
void Matriz::set_valuesColum(Vector _v1, Vector _v2, Vector _v3,Vector _v4){
	Vector vR,u1,u2,u3;
	vR.set_values(0.0,0.0,0.0,1.0);
    u1.set_values(_v1.getX(),_v2.getX(),_v3.getX(),_v4.getX());
    u2.set_values(_v1.getY(),_v2.getY(),_v3.getY(),_v4.getY());
    u3.set_values(_v1.getZ(),_v2.getZ(),_v3.getZ(),_v4.getZ());
    
	vectores[0] = u1;
	vectores[1] = u2;
	vectores[2] = u3;
	vectores[3] = vR;
}
Vector Matriz::getVector1(){
	return vectores[0];
}
Vector Matriz::getVector2(){
	return vectores[1];
}

Vector Matriz::getVector3(){
	return vectores[2];
}

Vector Matriz::getVector4(){
	return vectores[3];
}

Vector Matriz::getVector(int num){
	return vectores[num-1];
}

double Matriz::get(int x, int y){
	return vectores[x - 1].get(y);
}

void Matriz::setVector(int indice, Vector v){
	vectores[indice] = v;
}

void Matriz::setVector1(Vector v){
	vectores[0] = v;
}

void Matriz::setVector2(Vector v){
	vectores[1] = v;
}

void Matriz::setVector3(Vector v){
	vectores[2] = v;
}

void Matriz::setVector4(Vector v){
	vectores[3] = v;
}

/*std::string Matriz::toString(){
	std::string matriz ("");
	int i,j;

	for(i = 1; i < 5; i++){
		for(j = 1; j < 5; j++){
			matriz = matriz + std::to_string(get(j,i)) + " ";
		}
		matriz = matriz + '\n';
	}

	return matriz; 
}*/