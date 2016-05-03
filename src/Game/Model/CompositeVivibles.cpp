/*
 * Misiles.cpp
 *
 *  Created on: 1 de may. de 2016
 *      Author: ramon
 */

#include "CompositeVivibles.h"

using namespace std;

CompositeVivibles::CompositeVivibles() {
	vectorObjetos = vector<Vivible*>();
}

CompositeVivibles::~CompositeVivibles() {
	for (int var = 0; var < vectorObjetos.size(); ++var) {
		delete vectorObjetos[var];
	}
}

void CompositeVivibles::vivir(int x, int y){
	for (int var = 0; var < vectorObjetos.size(); ++var) {
		if(vectorObjetos[var]->aunVive()){
			vectorObjetos[var]->vivir(x,y);
		} else {
			/*Vivible* eliminar = vectorObjetos[var];
			vectorObjetos.
			delete eliminar;*/

		}
	}
	//this->mostrar(y);
}

void CompositeVivibles::agregarObjetoVivible(Vivible* &unObj){
	vectorObjetos.push_back(unObj);
}

/*void CompositeVivibles::mostrar(int velY){
	for (int var = 0; var < vectorObjetos.size(); ++var) {
		vectorObjetos[var]->mostrar(velY);
		}
}*/
