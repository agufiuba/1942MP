/*
 * Controller.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: keynaka
 */

#ifndef SRC_CONTROL_CONTROLLER_H_
#define SRC_CONTROL_CONTROLLER_H_

#include "SDL2/SDL.h"
#include "IController.h"
#include "ControllerMissiles.h"
#include "../model/Avion.h"
#include "../model/Resolucion.h"
#include "../events/CompanionEvent.h"
#include "../../libs/socket/Client.h"
#include "../../xml/conf/MisilConf.h"
#include "../../xml/conf/AvionConf.h"

#include <iostream>
#include <string>

class Controller : public IController {

private:
	Vivible* obj;
	int velX,velY;
	Resolucion* resolucionPantalla;
	int velocidadStandard;
	Client* cliente;
	MisilConf* misilConf;
	bool envioExplosion;

public:
	Controller(Vivible* unObj, SDL_Renderer* renderer, Resolucion* resolucion, Client* client);
	~Controller();
	void press(SDL_Event *event);
	void hacerVueltereta();
	void hacerVivir();
	Vivible* getVivible();

	void actualizarPosicionAvionEnServer();

	void setAmetralladora();
	void setBalaNormal();
	
	ControllerMissiles* controlDeMisiles;
};

#endif /* SRC_CONTROL_CONTROLLER_H_ */
