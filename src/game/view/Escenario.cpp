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

Escenario::Escenario(bool isFullScreen) {
	this->isFullScreen = isFullScreen;
	setResolucion();
	inicializar();
}

Escenario::Escenario(int width, int height) {
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	resolucion = Resolucion::INSTANCE(width, height);
	inicializar();
}

Escenario::Escenario(int width, int height, bool isFullScreen) {
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	this->isFullScreen = isFullScreen;
	resolucion = Resolucion::INSTANCE(width, height);
	inicializar();
}

Escenario::Escenario(int fps, int width, int height){
	this->FRAMES_PER_SECOND = fps;
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	resolucion = Resolucion::INSTANCE(width, height);
	inicializar();
}

void Escenario::inicializar() {
	window = NULL;
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
	fondoDePantalla = new Texture( gRenderer );
	if (!fondoDePantalla->loadFromFile( DIR_FONDO_PANTALLA )) {
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
	resolucion = Resolucion::INSTANCE();
	SCREEN_HEIGHT = resolucion->getHeightScreen();
	SCREEN_WIDTH = resolucion->getWidthScreen();
}

void Escenario::printErrorSDL(string error) {
	cout << "No se puede iniciarlizar "<< error << endl;
	cout << "SDL Error: " << SDL_GetError() << endl;
}

void Escenario::actualizarEscenario(Posicion* pos) {
	SDL_RenderClear(gRenderer);
	fondoDePantalla->render(pos->getX(), pos->getYsdl());

	for (int i = 0; i < fondosVivibles.size(); i++) {
		fondosVivibles[i]->vivir(0,0);
	}
	myControl->hacerVivir();
	controllers->hacerVivir();

	SDL_RenderPresent(gRenderer);
}

void Escenario::aplicarFPS(Uint32 start) {
	Uint32 dif = (SDL_GetTicks() - start);
	int time = 1000 / FRAMES_PER_SECOND;
	if(time > dif) {
		SDL_Delay( time - dif);
	}
}

void Escenario::setFondosVivibles() {

	Vivible* isla = new Isla(gRenderer, new Posicion(1150, 700), 1);
	Vivible* isla2 = new Isla(gRenderer, new Posicion(300, 2200), 2);
	Vivible* isla3 = new Isla(gRenderer, new Posicion(560, 3400), 3);
	Vivible* isla4 = new Isla(gRenderer, new Posicion(0, 4500), 1);
	Vivible* isla5 = new Isla(gRenderer, new Posicion(600, 5700), 2);
	Vivible* isla6 = new Isla(gRenderer, new Posicion(750, 6400), 3);
	Vivible* portaAvion = new Isla(gRenderer, new Posicion(500, 500), 4);

	fondosVivibles.push_back(isla);
	fondosVivibles.push_back(isla2);
	fondosVivibles.push_back(isla3);
	fondosVivibles.push_back(isla4);
	fondosVivibles.push_back(isla5);
	fondosVivibles.push_back(isla6);
	fondosVivibles.push_back(portaAvion);

}

void Escenario::setOtrosAviones() {
	char key[] = "key";
	char gon[] = "gon";
	char max[] = "max";

	Vivible* otroAvion1 = new Avion(key, gRenderer, resolucion, new Posicion(800, 100), amarillo);
	Vivible* otroAvion2 = new Avion(gon, gRenderer, resolucion, new Posicion(600, 100), rojo);
	Vivible* otroAvion3 = new Avion(max, gRenderer, resolucion, new Posicion(400, 100), verde);

	controllers = new HandlerPlayersControllers(gRenderer, resolucion);
	controllers->setPlayer((Avion*)otroAvion1);
	controllers->setPlayer((Avion*)otroAvion2);
	controllers->setPlayer((Avion*)otroAvion3);
}

void Escenario::run() {
	if (!inicioCorrectamente) {
		return ;
	}

	int pixelesArecorrer = CANTIDAD_SCREEN * SCREEN_HEIGHT;
	int screensRecorridos = 0;

	char ray[] = "ray";


	Vivible* unAvion = new Avion(ray, gRenderer, resolucion, new Posicion(200, 100), azul);

	setFondosVivibles();
	setOtrosAviones();

	myControl = new Controller(unAvion, gRenderer, resolucion);

	Posicion* posicionEscenario = new Posicion(0, pixelesArecorrer);
	actualizarEscenario(posicionEscenario);

	Uint32 start;
	bool quit = false;

	int i = 0;
	Posicion* posAvion = new Posicion(0,0);
	while (!quit) {

		start = SDL_GetTicks();
		while( SDL_PollEvent(&evento) != 0 ) {

			myControl->press(&evento);

			//TODO Aca se simula lo que seria el movimiento de otro jugador
			i++;
			//cout<<i<<endl;
			if (i > 15) {
//				cout << "Realizo movimiento arriba" << endl;
				controllers->mover("max",'U');
				controllers->mover("gon",'R');
				//Realizo disparo
				//controllers->mover("max",'S');
				//Realizo vueltera
				//controllers->mover("max",'E');
				i = 0;
			}

			if(evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q) {
				quit = true;
				break;
			}

		}

		bool isFinNivel = screensRecorridos >= CANTIDAD_SCREEN_TOTAL;

		if (!isFinNivel && posicionEscenario->getY() > SCREEN_HEIGHT) {
			posicionEscenario->mover(0,VELOCIDAD_SCREEN);

		} else {
			screensRecorridos += CANTIDAD_SCREEN;
			posicionEscenario->setPosicion(0, pixelesArecorrer);
			//setFondosVivibles();
		}

		actualizarEscenario(posicionEscenario);
		posAvion->setPosicion(unAvion->getX(), unAvion->getY());
		posAvion->print();
		aplicarFPS(start);

	}

	limpiarMemoria();
	delete posicionEscenario;
}

void Escenario::limpiarMemoria() {
	delete myControl;
	delete controllers;
}
