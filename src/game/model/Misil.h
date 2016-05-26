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
#include "../model/Resolucion.h"
#include "../model/Posicion.h"
#include "../../xml/conf/MisilConf.h"

using namespace std;

class Misil: public Vivible {
private:
	int anchoFondo, largoFondo;

	MisilView* vistaMisil;
	Posicion* posicion;
	bool viviendo;
	int dano;

	Timer* t;

	void mostrar();
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	Misil(SDL_Renderer * renderer, Posicion* posicion, Resolucion* resolucion, MisilConf* config);
	virtual ~Misil();

	void vivir(int x,int y);
	int getAnchoFondo();
	int getLargoFondo();
	bool aunVive();
	int getDano();

};

#endif /* SRC_GAME_MODEL_MISIL_H_ */
