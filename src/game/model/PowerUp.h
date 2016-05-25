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

//#include "../../xml/conf/PowerUpConf.h"

#include <string>

class PowerUp: public Vivible {
private:
	string id;
	string type;

	int anchoFondo, largoFondo;
	int velocidadStandard;
	bool viviendo;

	PowerUpView* vistaPowerUp;

	Posicion* posicion;

//	AvionConf* configuracion;

	void mostrar();
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	PowerUp(SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, string type, string id);
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

	void activarPowerUp();

	void vivir();
	bool aunVive();
};

#endif /* SRC_GAME_MODEL_POWERUP_H_ */
