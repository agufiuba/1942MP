/*
 * Escenario.h
 *
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "Texture.h"
#include "../model/Resolucion.h"
#include "../model/Posicion.h"
#include "../model/Avion.h"
#include "../controller/IController.h"
#include "../controller/Controller.h"
#include "../controller/PlayersController.h"
#include "../controller/Timer.h"
#include "../model/Vivible.h"
#include "Isla.h"
#include <vector>

class Escenario {

private:
	SDL_Window* window = NULL;
	Texture* fondoDePantalla = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Rect posicion;
	SDL_Event evento;
	Resolucion* resolucion;

	bool inicioCorrectamente;
	bool isFullScreen = true;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMES_PER_SECOND = 60;
	int CANTIDAD_SCREEN = 5;
	int CANTIDAD_SCREEN_TOTAL = 30;
	int VELOCIDAD_SCREEN = -3;

	const char* DIR_FONDO_PANTALLA = "src/game/images/fondoPantalla4000x4000Marcado.bmp";
	const char* WINDOW_TITLE = "1942 MultiPlayer";

	vector<IController*> controllersList;
	vector<Vivible*> fondosVivibles;

	void inicializar();
	void setResolucion();
	void printErrorSDL(string error);
	void actualizarEscenario(Posicion* posicion);
	void aplicarFPS(Uint32 start);
	void setFondosVivibles();

public:
	Escenario();
	Escenario(int fps);
	Escenario(int width, int height);
	Escenario(int fps, int width, int height);
	~Escenario();
	void run();
};

#endif /* ESCENARIO_H_ */
