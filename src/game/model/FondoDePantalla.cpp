/*
 * FondoDePantalla.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "FondoDePantalla.h"
using namespace std;

FondoDePantalla::FondoDePantalla() {
	window = NULL;
	fondoDePantalla = new Texture();

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
	fondoDePantalla->free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	gRenderer = NULL;
	window = NULL;
	SDL_Quit();
}

void FondoDePantalla::printErrorSDL(string error) {
	cout << "No se puede iniciarlizar "<< error << endl;
	cout << "SDL Error: " << SDL_GetError() << endl;
}

void FondoDePantalla::actualizarFondoDePantalla(int x, int y) {
	SDL_RenderClear(gRenderer);
	posicion.x = x;
	posicion.y = y;
	fondoDePantalla->render(x, y, gRenderer);
	SDL_RenderPresent(gRenderer);
}

int FondoDePantalla::moverFondoDePantalla(int y) {
	if (y > -1100) {
		cout << "baja y: " << y << endl;
		y-=3;
	}
	return y;
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

	int x = 0;
	int y = 0;
	actualizarFondoDePantalla(x, y);

	cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << endl;
	cout << "SCREEN_HEIGHT: " << SCREEN_HEIGHT << endl;
	cout << "actual: " << posicion.y << endl;
	Uint32 start;

	while (!quit) {
		start = SDL_GetTicks();
		while( SDL_PollEvent(&evento) != 0 ) {
			if(evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q) {
				quit = true;
				break;
			}

		}

		y = moverFondoDePantalla(y);
		actualizarFondoDePantalla(x, y);
		aplicarFPS(start);

	}
}

