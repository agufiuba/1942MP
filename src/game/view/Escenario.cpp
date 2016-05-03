/*
 * Escenario.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "Escenario.h"

using namespace std;

Escenario::Escenario(){
	setResolucion();
	inicializar();
}

Escenario::Escenario(int fps){
	this->FRAMES_PER_SECOND = fps;
	setResolucion();
	inicializar();
}

Escenario::Escenario(int width, int height) {
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	inicializar();
}

Escenario::Escenario(int fps, int width, int height){
	this->FRAMES_PER_SECOND = fps;
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	inicializar();
}

void Escenario::inicializar() {
	window = NULL;
	fondoDePantalla = new Texture();
	inicioCorrectamente = true;

	// Inicializar SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printErrorSDL("SDL");
		inicioCorrectamente = false;
	}

	// Inicializar window
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printErrorSDL("window");
		inicioCorrectamente = false;
	}

	// Inicializar window FullScreen
	if (isFullScreen && SDL_SetWindowFullscreen(window, SDL_TRUE) < 0) {
		printErrorSDL("Full Screen");
		inicioCorrectamente = false;
	}

	// Inicializar Renderer
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printErrorSDL("Renderer");
		inicioCorrectamente = false;
	}

	// Load fondo de pantalla
	if (!fondoDePantalla->loadFromFile(DIR_FONDO_PANTALLA, gRenderer)) {
		printErrorSDL("Fondo de Pantalla");
		inicioCorrectamente = false;
	}

}

Escenario::~Escenario() {
	resolucion->~Resolucion();
	fondoDePantalla->free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	gRenderer = NULL;
	window = NULL;
	SDL_Quit();
}

void Escenario::setResolucion() {
	resolucion = new Resolucion();
	SCREEN_HEIGHT = resolucion->getHeightScreen();
	SCREEN_WIDTH = resolucion->getWidthScreen();
}

void Escenario::printErrorSDL(string error) {
	cout << "No se puede iniciarlizar "<< error << endl;
	cout << "SDL Error: " << SDL_GetError() << endl;
}

void Escenario::actualizarEscenario(Posicion* pos) {
	SDL_RenderClear(gRenderer);
	fondoDePantalla->render(pos->getX(), pos->getYsdl(), gRenderer);
	SDL_RenderPresent(gRenderer);
}

void Escenario::aplicarFPS(Uint32 start) {
	Uint32 dif = (SDL_GetTicks() - start);
	int time = 1000 / FRAMES_PER_SECOND;
	if(time > dif) {
		SDL_Delay( time - dif);
	}
}

void Escenario::run() {
	if (!inicioCorrectamente) {
		return ;
	}

	int pixelesArecorrer = CANTIDAD_SCREEN * SCREEN_HEIGHT;
	int screensRecorridos = 0;

	Posicion* posicionEscenario = new Posicion(0, pixelesArecorrer);
	actualizarEscenario(posicionEscenario);

	cout << "actual: "; posicionEscenario->print();

	Uint32 start;
	bool quit = false;

	while (!quit) {
		start = SDL_GetTicks();
		while( SDL_PollEvent(&evento) != 0 ) {
			if(evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q) {
				quit = true;
				break;
			}

		}

		bool isFinNivel = screensRecorridos >= CANTIDAD_SCREEN_TOTAL;

		if (!isFinNivel && posicionEscenario->getY() > SCREEN_HEIGHT) {
			posicionEscenario->mover(0,VELOCIDAD_SCREEN);
			posicionEscenario->print();

		} else {
			screensRecorridos += CANTIDAD_SCREEN;
			posicionEscenario->setPosicion(0, pixelesArecorrer);
		}

		actualizarEscenario(posicionEscenario);
		aplicarFPS(start);

	}
	posicionEscenario->~Posicion();
}

