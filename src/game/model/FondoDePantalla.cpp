/*
 * FondoDePantalla.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "FondoDePantalla.h"

using namespace std;

FondoDePantalla::FondoDePantalla(){
	setResolucion();
	inicializar();
}

FondoDePantalla::FondoDePantalla(int fps){
	this->FRAMES_PER_SECOND = fps;
	setResolucion();
	inicializar();
}

FondoDePantalla::FondoDePantalla(int width, int height) {
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	inicializar();
}

FondoDePantalla::FondoDePantalla(int fps, int width, int height){
	this->FRAMES_PER_SECOND = fps;
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	inicializar();
}

void FondoDePantalla::inicializar() {
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
		// TODO: En el futuro sacar el isFullScreen
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

FondoDePantalla::~FondoDePantalla() {
	resolucion->~Resolucion();
	fondoDePantalla->free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	gRenderer = NULL;
	window = NULL;
	SDL_Quit();
}

void FondoDePantalla::setResolucion() {
	resolucion = new Resolucion();
	SCREEN_HEIGHT = resolucion->getHeightScreen();
	SCREEN_WIDTH = resolucion->getWidthScreen();
}

void FondoDePantalla::printErrorSDL(string error) {
	cout << "No se puede iniciarlizar "<< error << endl;
	cout << "SDL Error: " << SDL_GetError() << endl;
}

void FondoDePantalla::actualizarFondoDePantalla(Posicion* pos) {
	SDL_RenderClear(gRenderer);
	fondoDePantalla->render(pos->getX(), pos->getY(), gRenderer);
	SDL_RenderPresent(gRenderer);
}

void FondoDePantalla::aplicarFPS(Uint32 start) {
	Uint32 dif = (SDL_GetTicks() - start);
	int time = 1000 / FRAMES_PER_SECOND;
	if(time > dif) {
		SDL_Delay( time - dif);
	}
}

void FondoDePantalla::run() {
	if (!inicioCorrectamente) {
		return ;
	}

	Posicion* posicion = new Posicion(0, SCREEN_HEIGHT);
	actualizarFondoDePantalla(posicion);

	cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << endl;
	cout << "SCREEN_HEIGHT: " << SCREEN_HEIGHT << endl;
	cout << "actual: " << posicion->getX() << ", " << posicion->getY() << endl;
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

		if (posicion->getY() > -1070) {
			cout << "baja y: " << posicion->getY() << endl;
			posicion->moverVertical(-3);
		} else {
			//posicion->setPosicion(0, SCREEN_HEIGHT);
		}

		actualizarFondoDePantalla(posicion);
		aplicarFPS(start);

	}
	posicion->~Posicion();
}

