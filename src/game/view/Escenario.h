/*
 * Escenario.h
 *
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include <vector>
#include "Texture.h"
#include "Isla.h"
#include "../../libs/data/player_data.h"
#include "../model/Resolucion.h"
#include "../model/Posicion.h"
#include "../model/Avion.h"
#include "../model/Vivible.h"
#include "../controller/IController.h"
#include "../controller/Controller.h"
#include "../controller/PlayersController.h"
#include "../controller/Timer.h"
#include "../controller/HandlerPlayersControllers.h"
#include "../../xml/conf/GameConf.h"
#include "../../xml/parser/GameParser.h"
#include "../../xml/conf/EscenarioConf.h"
#include "../../libs/socket/Client.h"
#include "../../libs/xm_sdl/XM_SDL.h"
#include "../view/Screen.h"

class Escenario {

private:
	XM_SDL* sdl;
	Screen* escenarioScreen;
	SDL_Renderer* gRenderer = NULL;
	SDL_Event evento;
	Resolucion* resolucion;

	bool isFullScreen = false;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMES_PER_SECOND = 30;
	int CANTIDAD_SCREEN = 5;
	int CANTIDAD_SCREEN_TOTAL = 90;
	int VELOCIDAD_SCREEN = -3;
	int pixelesArecorrer = 4000;
	int pixelesRecorridos = 0;

	//Mas desfasaje, mas abajo se ponen las islas
	int desfasajeConexion = 70;

	string rojo = "rojo";
	string azul = "azul";
	string verde = "verde";
	string amarillo = "amarillo";
	const char* WINDOW_TITLE = "1942 MultiPlayer";

	string DIR_FONDO_PANTALLA;

	IController* myControl;
	HandlerPlayersControllers* controllers;
	vector<Isla*> fondosVivibles;
//	vector<SDL_Event*> eventosList;

	GameConf* gc;
	Client* unCliente;

	void setResolucion();
	void actualizarEscenario(Posicion* posicion);
	void aplicarFPS(Uint32 start);
	void setFondosVivibles(int x, int y);
	void configurarFondosVivibles();
	void limpiarFondosVivibles();
	void limpiarEventos();

public:
	Escenario(GameConf* configuracion, XM_SDL* sdl);
	~Escenario();
	SDL_Event* run();
	HandlerPlayersControllers* getHandler();
	void setClient(Client* cliente);
	void configurarAvionAmigo(PlayerData* playerData);
	void configurarMiAvion(PlayerData* playerData);

};

#endif /* ESCENARIO_H_ */
