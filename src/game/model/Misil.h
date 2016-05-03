/*
 * Misil.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_MODEL_MISIL_H_
#define SRC_GAME_MODEL_MISIL_H_

#include "Vivible.h"
#include "../controller/Timer.h"
#include "../view/MisilView.h"

using namespace std;

class Misil: public Vivible {
private:
	int anchoFondo, largoFondo; //TODO: esto estaria bueno poder sacarlo del screen

	MisilView* vistaMisil;
	int x, y;

	bool viviendo;

	Timer* t;

	void mostrar();
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	Misil(SDL_Renderer * renderer, int x, int y);
	virtual ~Misil();

	void vivir(int x,int y);
	int getAnchoFondo();
	int getLargoFondo();


  bool aunVive();
};

#endif /* SRC_GAME_MODEL_MISIL_H_ */
