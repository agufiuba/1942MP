/*
 * Vivible.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: keynaka
 */

#ifndef SRC_MODEL_VIVIBLE_H_
#define SRC_MODEL_VIVIBLE_H_

using namespace std;

class Vivible{
	public:
	   virtual ~Vivible(){};
	   virtual void vivir(int x, int y){};
	   virtual int getAncho(){return 0;};
	   virtual int getLargo(){return 0;};
	   virtual void setVelocidadStandard(int vel){};
	   virtual void mostrar(){};
	   virtual int getX(){return 0;};
	   virtual int getY(){return 0;};
	   virtual bool aunVive(){return true;};
};

#endif /* SRC_MODEL_VIVIBLE_H_ */
