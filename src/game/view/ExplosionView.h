/*
 * ExplosionView.h
 *
 *  Created on: 25/05/2016
 *      Author: gonzalo
 */

#ifndef EXPLOSIONVIEW_H_
#define EXPLOSIONVIEW_H_

#include <iostream>
#include <string>
#include <vector>
#include "SDL2/SDL.h"
#include "Screen.h"
#include "../model/Posicion.h"
#include "../model/Sound.h"

using namespace std;

class ExplosionView {

private:
	string id;
	Screen* screen;
	Sound* sonido;
	vector<string> screens;
	int screenActual = 0;
	int cantidadRepeticiones = 5;
	int cantidadScreen = (6 * cantidadRepeticiones);
	bool ejecutoSonido;

public:
	ExplosionView(string id, Screen* screen, Posicion* posicion);
	virtual ~ExplosionView();
	void explotar(Posicion* posicion);
	bool exploto();
};

#endif /* EXPLOSIONVIEW_H_ */
