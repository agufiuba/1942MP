/*
 * PowerUp.h
 *
 *  Created on: 24 de may. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_POWERUP_H_
#define SRC_GAME_MODEL_POWERUP_H_

#include "../view/PowerUpView.h"
#include "Vivible.h"
#include "Resolucion.h"
#include "../model/Avion.h"
#include "../model/Posicion.h"
#include "../controller/IController.h"
#include "../events/CompanionEvent.h"
#include "../model/Player.h"
#include "../model/Sound.h"


//#include "../../xml/conf/PowerUpConf.h"

#include <string>

class PowerUp: public Vivible {
private:
	string id;
	string type;

	int anchoFondo, largoFondo;
	int velocidadStandard;
	bool viviendo;

	IController* control;

	PowerUpView* vistaPowerUp;

	Posicion* posicion;
	Sound* soundGetPowerUp;

//	AvionConf* configuracion;

	void mostrar();
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	PowerUp(SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, IController* control, string type, string id);
	~PowerUp();

	string getId();

	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();

	//PowerUpConf* getConfiguracion(); TODO: Agregar la configuracion desde XML

	void setVelocidadStandard(int vel);

	char activarPowerUp();

	void vivir();
	bool aunVive();
	void morir();
};

#endif /* SRC_GAME_MODEL_POWERUP_H_ */
