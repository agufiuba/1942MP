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
	   virtual void mostrar(){};
};

#endif /* SRC_MODEL_VIVIBLE_H_ */
