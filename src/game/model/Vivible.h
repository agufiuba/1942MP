/*
 * Vivible.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: keynaka
 */

#ifndef SRC_MODEL_VIVIBLE_H_
#define SRC_MODEL_VIVIBLE_H_

#include <string>
#include <mutex>

using namespace std;

class Vivible{
	public:
	   bool viviendo;
	   virtual ~Vivible(){};
	   virtual void vivir(int x, int y){};
	   virtual int getAncho(){return 0;};
	   virtual int getLargo(){return 0;};
	   virtual void mostrar(){};
	   virtual int getX(){return 0;};
	   virtual int getY(){return 0;};
	   virtual bool aunVive(){return true;};
	   virtual void desconectar(){};
	   virtual void conectar(){};

	   virtual string getId(){return 0;};

	   virtual void setVelocidadStandard(int vel){};
	   virtual void inicializoVueltereta(){};
	   virtual bool haciendoVueltereta(){return true;};

	   virtual bool tieneHP(){return true;};
	   virtual void setHP(int hp) {};

	   virtual void inicializoEstacionar(){};

	   virtual void setAmetralladora() {};

	   virtual bool estaEstacionando(){return true;};

	int posX;
	int posY;
	mutex theMutex;	
};

#endif /* SRC_MODEL_VIVIBLE_H_ */
