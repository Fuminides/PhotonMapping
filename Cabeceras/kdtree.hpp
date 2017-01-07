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
//#include "pila.hpp"
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
	 * Devuelve los k fotones mas proximos dado un punto.
	 */
	friend std::vector<Foton> knearest(const kdtree& arbol, Punto p);
	
	/** DE MOMENTO NO HACEN FALTA
	friend void iniciarIterador(kdtree<Foton> &a);
    friend bool existeSiguiente(kdtree<Foton> &a);
    friend Foton siguiente(kdtree<Foton> &a, bool &error);
	**/
   
 private:
	struct NodoBinario {
       Plano plano;
       Foton dato;
       NodoBinario* izq;
       NodoBinario* dcho;
	};

	NodoBinario* nodo; //Raíz
    int tamanyo;//Número de elementos
    //Pila<NodoBinario*> iter; //Iterador/Pila
	
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

/*
 * Devuelve los k fotones mas proximos dado un punto.
 */
std::vector<Foton> knearest(const kdtree& arbol, Punto p);

/** TODO ESTO QUE VIENE A CONTINUACION LO COMENTO PORQUE SI NO HACE FALTA DE MOMENTO NO HE PROBADO SU FUNCIONAMIENTO **/
/**
/* Se crea un elemento de tipo Pila que será el que gestionará el listado
   de los elementos, primero establece la raiz del árbol en 'nodo' y
   posteriormente va apilando los elementos en la pila hasta que un árbol deja
   de tener elementos en su subarbol izquierdo 
void iniciarIterador(kdtree<Foton> &a) {
  //crea pila vacía de punteros a nodos del árbol
	crear<typename kdtree<Foton>::NodoBinario*>(a.iter);
  //raíz del árbol en aux
	typename kdtree<Foton>::NodoBinario* aux=a.nodo; 

	while(aux!=nullptr) {
    //apila el puntero aux (a un nodo del árbol) en la pila del iterador
		apilar(a.iter,aux);  
		aux=aux->izq;
	}


/* Comprueba si está vacia la pila del iterador y devuelve true en el caso de
   que esté vacía y false en el caso de que esté vacia. 
bool existeSiguiente(kdtree<Foton> &a) {
  //hay siguiente si la pila del iterador es no vacía 
	return !(esVacia<typename kdtree<Foton>::NodoBinario*>(a.iter)); 
}

/* Recorre elemento a elemento, comprueba primero si existe elemento siguiente,
   si existe, primero almacena la cima y la desapila, posteriormente volverá a
   apilar los elementos desapilados y devolverá el dato 'unDato' que previamente
   hemos almacenado. 
Foton siguiente(kdtree<Foton> &a, bool &error) {
	typename kdtree<Foton>::NodoBinario* aux;
	Foton unDato;
	if(existeSiguiente(a)) {
		error=false;
		aux=cima<typename kdtree<Foton>::NodoBinario*>(a.iter); 
		desapilar<typename kdtree<Foton>::NodoBinario*>(a.iter); 
    //Siguiente elemento visitado
		unDato=aux->dato; 
		aux=aux->dcho;
		while(aux!=nullptr) {
			apilar<typename kdtree<Foton>::NodoBinario*>(a.iter,aux);
			aux=aux->izq;
		}
	}
	else {
		error=true;
	}
	return unDato;
}**/

#endif

