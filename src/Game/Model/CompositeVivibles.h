/*
 * Misiles.h
 *
 *  Created on: 1 de may. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_MODEL_COMPOSITEVIVIBLES_H_
#define SRC_GAME_MODEL_COMPOSITEVIVIBLES_H_

#include "Vivible.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class CompositeVivibles:public Vivible{
	public:
		CompositeVivibles();
		virtual ~CompositeVivibles();
		void agregarObjetoVivible(Vivible* &unObj);
		void vivir(int x, int y);
		void mostrar();
	private:
		vector<Vivible*> vectorObjetos = NULL;
};


#endif /* SRC_GAME_MODEL_COMPOSITEVIVIBLES_H_ */
