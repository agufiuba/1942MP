/*
 * ChispazoView.h
 *
 *  Created on: 20/06/2016
 *      Author: gonzalo
 */

#ifndef CHISPAZOVIEW_H_
#define CHISPAZOVIEW_H_

#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include "Screen.h"
#include "../model/Posicion.h"

using namespace std;

class ChispazoView {

private:
	string id;
	Screen* screen;
	string imagen;

public:
	ChispazoView(string id, Screen* screen, Posicion* posicion);
	virtual ~ChispazoView();
};

#endif /* CHISPAZOVIEW_H_ */
