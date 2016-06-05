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
	int eliminar = -1;
	for (int var = 0; var < vectorObjetos.size(); ++var) {
		Vivible* obj= vectorObjetos[var];
		if(obj->aunVive()){
			obj->vivir(x,y);
		} else {
			eliminar = var;
		}
	}
	// if (eliminar >= 0 ){
	// 	Vivible* objEliminar = vectorObjetos[eliminar];
	// 	delete objEliminar;
	// 	vectorObjetos.erase(vectorObjetos.begin()+eliminar);
	// }
}

void CompositeVivibles::agregarObjetoVivible(Vivible* &unObj){
	vectorObjetos.push_back(unObj);
}
