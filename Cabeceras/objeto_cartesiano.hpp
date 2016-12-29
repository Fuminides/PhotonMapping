/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a cualquier objeto en el espacio cartesiano con coordenadas homogeneas.
 */
#ifndef objeto_cartesiano_h_
#define objeto_cartesiano_h_

#include <string>

class ObjetoCartesiano {
 	protected:
	 	double valores[4];

	public:
	 	void set_values(double, double, double);
	 	void set_values(double, double, double, double);

	 	/*
		 * Setters.
		 */
	 	void set_X(double);
	 	void set_Y(double);
	 	void set_Z(double);
        void set_D(double);
		/*
		 * Getters.
		 */
	 	double getX();
	 	double getY();
	 	double getZ();
	 	double getD();

	 	double get(int indice);

	 	void multiplicar(double);
	 	virtual bool esVector() = 0; 

	 	std::string to_string();
 };

#endif