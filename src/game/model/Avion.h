#ifndef SRC_MODEL_AVION_H_
#define SRC_MODEL_AVION_H_

#include "../view/AvionView.h"
#include "Vivible.h"
#include "../controller/Timer.h"
#include "Resolucion.h"
#include "../model/Posicion.h"
#include "../../libs/data/player_data.h"
#include "../../xml/conf/AvionConf.h"
#include <string>

class Avion: public Vivible {
private:
	string id;
	int anchoFondo, largoFondo;
	int velocidadStandard;
	bool viviendo;

	bool realizandoVueltereta;
	Timer* t;

	AvionView* vistaAvion;
	Posicion* posicion;
	AvionConf* configuracion;


	void realizoVueltereta();
	void mostrarVueltereta(int frame);

	void mostrar(int velX);
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	Avion(PlayerData* playerData, SDL_Renderer * &renderer, Resolucion* &resolucion, Posicion* posicionInicial, AvionConf* conf);
	~Avion();

	string getId();
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();
	AvionConf* getConfiguracion();

	void setVelocidadStandard(int vel);
	bool haciendoVueltereta();
	void inicializoVueltereta();

	void vivir(int velX, int velY);
	bool aunVive();

	void desconectar();
};

#endif /* SRC_MODEL_AVION_H_ */
