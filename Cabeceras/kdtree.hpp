/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa un kdtree de fotones.
 */


#ifndef TREE_H
#define TREE_H

#include <iostream>
#include "foton.hpp"
#include "punto.hpp"
#include "plano.hpp"
#include <vector>
#include <math.h>
#include <vector>
using namespace std;


struct kdtree{
	/* 
	 * Crea un kdtree sin ningún foton, vacío
	 */
    friend void crear(kdtree& arbol);
	/* 
	 * Ha insertado el foton 'e' en el kdtree 'arbol'. Si había un
	 * foton igual a 'e' previamente, lo ha sobreescrito
	 */
	friend void anyadir(kdtree& arbol,Foton e);
	/* 
	 * Devuelve verdad si y solo si en el kdtree 'arbol' hay un
     * foton igual al foton 'e'. En caso contrario devuelve falso
	 */
	friend bool esta(const kdtree& arbol,Foton e);
	/* 
	 * Si en 'arbol' hay un foton igual a 'e' devuelve ese foton, en caso 
     * contrario el parámetro error que se pasa por referencia se pondrá a true
	 */
	friend Foton obtener(const kdtree& arbol, Foton e,bool &error);
	/* 
	 * Devuelve el número de elementos que hay en el kdtree 'arbol'
	 */
	friend int tamanyo(const kdtree& arbol);
	/* 
	 * Devuelve true y solo si el kdtree 'arbol' no tiene ningún foton, 
	 * en caso contrario devuelve falso
	 */
	friend bool esVacia(const kdtree& arbol);
	/* 
	 * Si en 'arbol' hay un foton igual a 'e', borra ese foton y devuelve 
     * true. En caso de que no hay ningun foton igual a 'e' devuelve false
	 */
	friend bool borrar(kdtree& arbol, Foton e);
	/*
	 * Devuelve los k fotones mas proximos dado un punto.
	 */
	friend std::vector<Foton> knearest(kdtree& arbol, Punto p,int k);
   
 public:
	struct NodoBinario {
       Plano plano;
       Foton dato;
       NodoBinario* izq;
       NodoBinario* dcho;
	};

	NodoBinario* nodo; //Raíz
    int tamanyo;//Número de elementos
	
	/* 
	 * Ha insertado recursivamente el elemento 'e' a partir de 'nodo', que se 
     * encuentra en el nivel 'nivel'. Si el elemento ya se encontraba a partir de 
     * 'nodo', lo ha sobreescrito y no ha modificado 'tamanyo'. En caso contrario
     * ha insertado el elemento y ha modificado 'tamanyo'
	 */
	friend void insertarRec(typename kdtree::NodoBinario*& nodo, Foton e, int nivel);
	/* 
	 * Devuelve 1 si los fotones e y e2 son iguales y 0 en caso contrario
	 */ 
	friend int comparar(Foton e,Foton e2);
	/* 
	 * Ha buscado de forma recursiva el elemento 'e' a partir del nodo 'nodo' que se
     * encuentra en el nivel 'nivel'. Si ha encontrado un elemento igual a 'e' ha 
     * devuelto true, en caso contrario false
	 */
	friend bool estarRec(typename kdtree::NodoBinario* nodo, Foton e);
	/* 
	 * Ha devuelto el elemento igual a 'e' si se encontraba a partir del nodo 'nodo'
     * que se encuentra en el nivel 'nivel', poniendo además error a false. En caso
     * de no encontrarlo, y por lo tanto no haberlo podido, ha puesto error a true
	 */
	friend Foton obtenerRec(typename kdtree::NodoBinario* nodo, Foton e, bool &error);
	/* 
	 * Ha modificado el valor 'max' con el valor de 'dato' y posteriormente a 
	 * invocado a buscado en el subárbol izquierdo en el caso de que nivel&2 no 
	 * coincida con dimension en subárbol izquierdo y en el subárbol derecho un
	 * elemento mayor que este con respecto a la dimension 'dimension'. Se tiene 
	 * en cuenta esto, porque si la dimensión coincide con nivel%2, el elemento
	 * máximo para esa dimensión dada ha de estar en el subárbol derecho
	 */

	friend void max(typename kdtree::NodoBinario* nodo, Foton &max, int nivel, int dimension);
	/* 
	 * Primero comprueba si nodo es distinto de nullptr, ya que si es igual 
	 * significa que hemos llegado al fin de la búsqueda
	 * En caso de que no sea igual, mira si 'dato' es mayor o igual para la 
	 * dimensión 'dimensión' de 'dato', modificando en este caso su valor.
	 * Posteriormente ha seguido buscando un elemento mayor realizando una llamada
	 * recursiva, mirando primero si nivel%2 coincide con dimension para no tener 
	 * que buscar en el subárbol izquierdo o sí debido a la estructura del kdtree
	 * y luego ha realizado una llamada recursiva para buscar en subárbol derecho
	 */
	friend void maxRec(typename kdtree::NodoBinario* nodo, Foton &max, int nivel, int dim);
	/*
	 * Asigna en min el foton mas proximo a un punto.
	 */
	friend Foton nearestRec(typename kdtree::NodoBinario*& nodo,Punto p,double& dMin,Foton& min);
	/* 
	 * Ha borrado el foton 'e', en caso de encontrarlo búscandolo a partir del 
     * nodo 'nodo' que se encuentra en el nivel 'nivel, modificando 'tamanyo' y 
	 * poniendo borrado a true. En caso de no haberlo encontrado a puesto borrado
     * a false 
	 */
	friend void borrarRec(typename kdtree::NodoBinario*& nodo, Foton e, int nivel, int& tamanyo, bool &borrado);

	
};

/* Crea un kdtree 'arbol' poniendo la raíz(nodo) a nulo y el tamaño 
 *  del kdtree a 0, de forma que indica que está vacío
 */
typedef struct kdtree arbol;
void crear(kdtree& arbol) {
  arbol.nodo=nullptr;
  arbol.tamanyo=0;
}


/* Utiliza la función insertarRec() para insertar recursivamente el elemento 'e'
 *dado en el kdtree 'arbol', pasándole la raiz 'nodo', el elemento 'e', el nivel
 *por el que va, en este caso 0 ya que es el inicio y el tamaño 'tamanyo' 
 *del kdtree  
 */

void anyadir(kdtree& arbol,Foton e) {
    insertarRec(arbol.nodo,e, 0);
	  arbol.tamanyo+=1;
}

/* - Inserta recursivamente el elemento 'e' en nodo 'nodo', con nivel 'nivel' 
 *  y con tamaño 'tamanyo'. 
 *  - Para ello comprueba si el nodo es vacío primero, en el caso de que sea vacío 
 *  le asigna el dato y pone los punteros 'izq' y 'dcho' a null, y aumenta 
 *  el tamaño. 
 *  - Si no es vacio la forma de inserción es la de un kd-tree, 
 *  considerando el nivel i del árbol, es decir del campo 'dato' de 'nodo',
 *  si el elemento 'e' a insertar tiene dimension (en el campo i%2) menor o igual 
 *  a la del nodo en ese mismo campo, se inserta en el subarbol izquierdo, si es
 *  superior en el derecho.
 */

void insertarRec(typename kdtree::NodoBinario*& nodo, Foton e, int nivel) {
    if (nodo==nullptr)   {
        nodo=new typename kdtree::NodoBinario;
        nodo->dato=e;
        nodo->izq=nullptr;
        nodo->dcho=nullptr;
        Vector normal;
        if ( nivel % 2 == 0 ){
			normal.set_values(0,0,1);
		}
        else{
			normal.set_values(0,1,0);
		}
		//No me deja pasarle un vector nulo por lo tanto le paso uno con todo 0.
		Vector n;
		n.set_values(0.0, 0.0, 0.0);
        nodo->plano.set_values(e.getPosicion(), normal, n, 0.0, n, 0.0);
    }
    else {
        if(nodo->plano.derecha(e.getPosicion())) {
            insertarRec(nodo->dcho,e, nivel+1);
        }
        else {
            insertarRec(nodo->izq,e, nivel + 1);
        }
    }
}

/* 
 * Devuelve 1 si los fotones e y e2 son iguales y 0 en caso contrario
 */ 
int comparar(Foton e, Foton e2){
	if(e.getPosicion().equals(e2.getPosicion())){
		return 1;
	}
	else {
		return 0;
	}
}

/* Comprueba si el elemento 'e' se encuentra en el kdtree. Para ello realiza una
 *  llamada estarRec, indicando a partir de donde tiene que buscar,nodo(que es 
 *  la raíz), el elemento a buscar 'e' y en el nivel en que se encuentra 
 */

bool esta(const kdtree& arbol,Foton e){
    return estarRec(arbol.nodo,e);
}

/* Busca recursivamente el elemento 'e' a partir de 'nodo' que se encuentra en
 *  el nivel 'nivel'
 *  Primero se comorueba si es nulo 'nodo', en cuyo caso significa que 'e' no 
 *  está por lo que se devuelve falso.
 *  En caso de que el dato de 'nodo' sea igual a 'e' se devuelve true.
 *  Si no se cumple ninguna de estas dos condiciones se busca en el subárbol 
 *  izquierdo o derecho, en función de la dimensión de 'e' con respecto a la de 
 *  'dato', siguiendo de esta manera la estructura de un kdtree 
 */

bool estarRec(typename kdtree::NodoBinario* nodo, Foton e) {
    if(nodo==nullptr){
        return false;
    }
    else {
        if(comparar(e, nodo->dato)==1){
            return true;
        }
        else {
            if(nodo->plano.derecha(e.getPosicion())){
                return estarRec(nodo->dcho,e);
            }
            else {
                return estarRec(nodo->izq,e);
            }
        }
    }
}

/* 
 * Devuelve el elemento que se encuentra en 'arbol' que es igual a 'e' si existe
 * uno que se igual, sino pone error a false. Para ello realiza una llamada a 
 *  la función obtenerRec, siendo arbol.nodo el nodo a partir del cual tiene que
 *  buscar (la raíz), 0 el nivel en que se encuentra, 'e' el elemento a buscar y
 *  la variable error para su posible modificación 
 */

Foton obtener(const kdtree& arbol, Foton e, bool &error) {

  return obtenerRec(arbol.nodo,e,error);;
}

/* Sigue una estructura parecida a la función estaRec.
 * Primero comprueba si el nodo es nulo, lo cual significa que hay que poner
 * error a true, devolviendo 'e' ya que hay que devolver un elemento. 
 * En caso de que 'e' sea igual a 'dato', devuelve este último y pone error a
 * false, ya que se ha obtenido el elemento igual a 'e'.
 * Por último, en caso de no localizarlo en este nodo, se busca en su subárbol
 * izquierdo o derecho dependiende la dimensión de 'e' respecto a la de 'dato',
 * siguiendo lo que es la estructura de un kdtree 
 */
Foton obtenerRec(typename kdtree::NodoBinario* nodo, Foton e, bool &error) {
  if(nodo==nullptr){
    error=true;
    return e;
  }
  else if(comparar(e, nodo->dato)==1){
    Foton nuevo=nodo->dato;
    error=false;
    return nuevo;
  }
  else {
    if(nodo->plano.derecha(e.getPosicion())){
      return obtenerRec(nodo->dcho,e,error);
    }
    else {
      return obtenerRec(nodo->izq,e,error);
    }
  }
}
/* 
 * Compara el valor del campo 'tamanyo' del kdtree 'arbol' con 0, si son iguales
 * indica que está vacio y devuelve true, en el caso que tenga otro valor 
 * devolverá false
 */
bool esVacia(const kdtree& arbol) {
  return arbol.tamanyo==0;
}

/* 
 * Devuelve el campo 'tamanyo' del kdtree 'arbol' 
 */
int tamanyo(const kdtree& arbol){
  return arbol.tamanyo;
}

/** A PARTIR DE AQUI SE HAN CREADO TODAS LAS FUNCIONES PARA DEVOLVER LOS K MAS CERCANOS**/

/* 
 * Ha modificado el valor 'max' con el valor de 'dato' y posteriormente a 
 * invocado a buscado en el subárbol izquierdo en el caso de que nivel&2 no 
 * coincida con dimension en subárbol izquierdo y en el subárbol derecho un
 * elemento mayor que este con respecto a la dimension 'dimension'. Se tiene 
 * en cuenta esto, porque si la dimensión coincide con nivel%2, el elemento
 * máximo para esa dimensión dada ha de estar en el subárbol derecho
 */

void max(typename kdtree::NodoBinario* nodo, Foton &max, int nivel, int dimension) {
  max=nodo->dato;
  if(nivel%2!=dimension) {
    maxRec(nodo->izq,max, nivel+1,dimension);
  }
  maxRec(nodo->dcho,max,nivel+1,dimension);
}

/* 
 * Primero comprueba si nodo es distinto de nullptr, ya que si es igual 
 * significa que hemos llegado al fin de la búsqueda
 * En caso de que no sea igual, mira si 'dato' es mayor o igual para la 
 * dimensión 'dimensión' de 'dato', modificando en este caso su valor.
 * Posteriormente ha seguido buscando un elemento mayor realizando una llamada
 * recursiva, mirando primero si nivel%2 coincide con dimension para no tener 
 * que buscar en el subárbol izquierdo o sí debido a la estructura del kdtree
 * y luego ha realizado una llamada recursiva para buscar en subárbol derecho
 */

void maxRec(typename kdtree::NodoBinario* nodo, Foton &max, int nivel, int dim) {
  if(nodo!=nullptr) {
	if(!nodo->plano.derecha(max.getPosicion())){
      //Así obtenemos el max que esté más abajo
      max=nodo->dato;
    }
    
    if(nivel&2!=dim){
      maxRec(nodo->izq,max,nivel+1,dim);
    }
    maxRec(nodo->dcho,max,nivel+1,dim);
  }
}

/* 
 * Si en 'arbol' hay un foton igual a 'e', borra ese foton y devuelve 
 * true. En caso de que no hay ningun foton igual a 'e' devuelve false
 */
bool borrar(kdtree& arbol, Foton e){
	bool borrado=false;
	borrarRec(arbol.nodo,e,0,arbol.tamanyo,borrado);
	return borrado;
}

/* 
 * Ha borrado el foton 'e', en caso de encontrarlo búscandolo a partir del 
 * nodo 'nodo' que se encuentra en el nivel 'nivel, modificando 'tamanyo' y 
 * poniendo borrado a true. En caso de no haberlo encontrado a puesto borrado
 * a false 
 */
void borrarRec(typename kdtree::NodoBinario*& nodo, Foton e, int nivel, int& tamanyo, bool &borrado){
	if(nodo!=nullptr) {
		if(comparar(e, nodo->dato)==1) {
			if(nodo->izq==nullptr && nodo->dcho==nullptr) {
				delete nodo;
				nodo=nullptr;
				tamanyo--;
				borrado=true;
			}
			else if(nodo->izq!=nullptr) {
				max(nodo->izq,nodo->dato,nivel+1,nivel%2);
				//Intercambio de elementos de nodos
				borrarRec(nodo->izq,nodo->dato,nivel+1,tamanyo,borrado);
			}

		  else {
			//nodo->izq==nullptr && nodo->dcho!=nullptr
			max(nodo->dcho,nodo->dato,nivel+1,nivel%2);
			//Intercambio de subárboles
			nodo->izq=nodo->dcho;
			nodo->dcho=nullptr;
			borrarRec(nodo->izq,nodo->dato,nivel+1,tamanyo,borrado);
		  }
		}
		else {
			if(nodo->plano.derecha(e.getPosicion())) {
				borrarRec(nodo->dcho,e,nivel+1,tamanyo,borrado);
			}
			else {
				borrarRec(nodo->izq,e,nivel+1,tamanyo,borrado);
			}
		}
	}
}
/*
 * Devuelve los k fotones mas proximos dado un punto.
 */
std::vector<Foton> knearest(kdtree& arbol, Punto p,int k){
	vector<Foton> listado;
	int num=0;
	Foton min=(arbol.nodo)->dato;
	Punto actual = min.getPosicion();
	double dMin=sqrt(pow((actual.getX()-p.getX()),2.0) + pow((actual.getY()-p.getY()),2.0) + pow((actual.getZ()-p.getZ()),2.0));
	while(num < k && tamanyo(arbol)>=k){
		//Iniciamos recorrido del arbol 
		num++;
		listado.push_back(nearestRec(arbol.nodo,p,dMin,min));
		//borrar(arbol,min);
	}

  return listado;
}


/*
 * Asigna en min el foton mas proximo a un punto.
 */
Foton nearestRec(typename kdtree::NodoBinario*& nodo, Punto p,double& dMin,Foton min){
	if(nodo != nullptr){
		Foton actual=nodo->dato;
		Punto act = min.getPosicion();
		double d=sqrt(pow((act.getX()-p.getX()),2.0) + pow((act.getY()-p.getY()),2.0) + pow((act.getZ()-p.getZ()),2.0));
		if(d<dMin){
			nearestRec(nodo->dcho,p,d,actual);
			nearestRec(nodo->izq,p,d,actual);
		}
		else{
			nearestRec(nodo->dcho,p,dMin,min);
			nearestRec(nodo->izq,p,dMin,min);
		}
	}

  return min;
	
}

#endif

