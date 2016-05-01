/*
 * Escenario.h
 *
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "../examples/libs/Texture.h"
#include "../model/Resolucion.h"
#include "../model/Posicion.h"

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
	int SCREEN_REPLAY = 2;
	int SCREEN_REPLAY_TOTAL = 6;

	const char* DIR_FONDO_PANTALLA = "src/game/images/fondoPantallaMarcado.bmp";
	const char* WINDOW_TITLE = "1942 MultiPlayer";

	void inicializar();
	void setResolucion();
	void printErrorSDL(string error);
	void actualizarEscenario(Posicion* posicion);
	void aplicarFPS(Uint32 start);

public:
	Escenario();
	Escenario(int fps);
	Escenario(int width, int height);
	Escenario(int fps, int width, int height);
	~Escenario();
	void run();
	//void agregar(const char*, Posicion* posicion);

};

#endif /* ESCENARIO_H_ */
