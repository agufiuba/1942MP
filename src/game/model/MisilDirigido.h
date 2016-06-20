/*
 * MisilDirigido.h
 *
 *  Created on: 14 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_MISILDIRIGIDO_H_
#define SRC_GAME_MODEL_MISILDIRIGIDO_H_

#include "Vivible.h"
#include "../controller/Timer.h"
#include "../view/MisilView.h"
#include "../model/Resolucion.h"
#include "../model/Posicion.h"
#include "../model/Sound.h"
#include "../../xml/conf/MisilConf.h"

using namespace std;

class MisilDirigido: public Vivible {
private:
	int anchoFondo, largoFondo;
	int dano;
	MisilView* vistaMisil;
	Posicion* posicion;
	Sound* sonido;

	Posicion* vectorVelocidad;

	Timer* t;

	void mostrar();
	void mover();

public:
	MisilDirigido(SDL_Renderer * renderer, Posicion* posicion, Resolucion* resolucion, MisilConf* config, int posicionApuntadoX, int posicionApuntadoY);
	virtual ~MisilDirigido();

	void vivir(int x,int y);
	int getX();
	int getY();
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	bool aunVive();
	int getDano();

	void setAmetralladora();
	void morir();
	Posicion* apuntar(int posicionApuntadoX, int posicionApuntadoY, int velocidad);
};


#endif /* SRC_GAME_MODEL_MISILDIRIGIDO_H_ */
