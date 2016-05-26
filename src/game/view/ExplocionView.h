/*
 * ExplocionView.h
 *
 *  Created on: 25/05/2016
 *      Author: gonzalo
 */

#ifndef EXPLOCIONVIEW_H_
#define EXPLOCIONVIEW_H_

#include <iostream>
#include <string>
#include <vector>
#include "SDL2/SDL.h"
#include "Screen.h"
#include "../model/Posicion.h"

using namespace std;

class ExplocionView {

private:
	string id;
	Screen* screen;
	vector<string> screens;
	int screenActual = 0;
	int cantidadRepeticiones = 5;
	int cantidadScreen = (6 * cantidadRepeticiones);

public:
	ExplocionView(string id, Screen* screen, Posicion* posicion);
	virtual ~ExplocionView();
	void explotar(Posicion* posicion);
	bool exploto();
};

#endif /* EXPLOCIONVIEW_H_ */
