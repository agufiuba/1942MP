/*
 * Misiles.cpp
 *
 *  Created on: 1 de may. de 2016
 *      Author: ramon
 */

#include "CompositeVivibles.h"

using namespace std;

CompositeVivibles::CompositeVivibles() {
	vectorObjetos = new vector<Vivible*>();
}

CompositeVivibles::~CompositeVivibles() {
	for (int var = 0; var < vectorObjetos.size(); ++var) {
			delete vectorObjetos[var];
		}
	delete vectorObjetos;
}

void CompositeVivibles::vivir(int x, int y){
	for (int var = 0; var < vectorObjetos.size(); ++var) {
		vectorObjetos[var]->vivir(x,y);
	}
}

void CompositeVivibles::agregarObjetoVivible(Vivible* &unObj){
	vectorObjetos.push_back(unObj);
}

void CompositeVivibles::mostrar(){
	for (int var = 0; var < vectorObjetos.size(); ++var) {
		vectorObjetos[var]->mostrar();
		}
}
