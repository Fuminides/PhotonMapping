#include "leer_ply.hpp"

int triangles2;
int poligonos;

void nextLine(ifstream * istream, string * str){
	while ((istream->peek()!='\n') && (*istream>>*str));
}

void leer(string nombre, Triangulo * v) {
  ifstream myfile;
  string word, dump;
  myfile.open (nombre);
  Punto p1, p2, p3;
  int nTr = 0;
  Punto vertexes[1000];

  do{
  	myfile >> word;
  	if (word.compare("element") == 0){
  		myfile >> word;
  		if ( word.compare("vertex")==0){
  			myfile >> nTr;
  		}
      if ( word.compare("face")==0){
        myfile >> poligonos;
      }
  	}
  	nextLine(&myfile, &dump);
  } while ((!word.compare("end_header")==0) && myfile.good());

  for (int i = 0; i < nTr; ++i)
  {
  	vertexes[i] = leerPunto(&myfile);
  }

  for (int i = 0 ; i < poligonos; i += 1)
  {
    int indi,indj,indz;

    myfile >> dump >> indi >> indj >> indz;
    v[i].set_values(vertexes[indi],vertexes[indj],vertexes[indz]);
  }

  myfile.close();
}

Punto leerPunto(ifstream * file){
	double coord[3];
	Punto p;

	*file >> coord[0] >> coord[1] >> coord[2];
  	p.set_values(coord[0], coord[1], coord[2]);

  	return p;
}

int nTriangulos(){ return triangles2; }
int nPoligonos() { return poligonos; }