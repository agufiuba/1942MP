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

class Escenario {

private:
	SDL_Window* window = NULL;
	Texture* fondoDePantalla = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Rect posicion;
	SDL_Event evento;
	Resolucion* resolucion;

	bool inicioCorrectamente;
	bool isFullScreen = false;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMES_PER_SECOND = 30;
	int CANTIDAD_SCREEN = 5;
	int CANTIDAD_SCREEN_TOTAL = 30;
	int VELOCIDAD_SCREEN = -3;

	/*
	enum Color { rojo = "rojo",
							azul = "azul",
							verde = "verde",
							amarillo = "amarillo",
							desconocido = "desconocido"
	};*/

	string rojo = "rojo";
	string azul = "azul";
	string verde = "verde";
	string amarillo = "amarillo";
	string desconocido = "desconocido";


	const char* DIR_FONDO_PANTALLA = "src/game/images/fondos/fondoPantalla4000x4000.bmp";
	const char* WINDOW_TITLE = "1942 MultiPlayer";

	IController* myControl;
	HandlerPlayersControllers* controllers;
	vector<Vivible*> fondosVivibles;

	void inicializar();
	void setResolucion();
	void printErrorSDL(string error);
	void actualizarEscenario(Posicion* posicion);
	void aplicarFPS(Uint32 start);
	void setFondosVivibles();
	void setOtrosAviones();
	void limpiarMemoria();
	void limpiarFondosVivibles();
	GameConf* gc;

public:
	Escenario();
	Escenario(int fps);
	Escenario(bool isFullScreen);
	Escenario(int width, int height);
	Escenario(int width, int height, bool isFullScreen);
	Escenario(int fps, int width, int height);
	~Escenario();
	SDL_Event* run();
	void configurarJugador(PlayerData* jugador);

};

#endif /* ESCENARIO_H_ */
