/*
 * Escenario.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "Escenario.h"
#include "../../xml/parser/GameParser.h"
using namespace std;

Escenario::Escenario() {
	setResolucion();
	inicializar();
}

Escenario::Escenario(int fps) {
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

Escenario::Escenario(GameConf* configuracion) {
	this->gc = configuracion;

	if (gc->escenario->alto != 0 && gc->escenario->ancho != 0) {
		this->SCREEN_WIDTH = gc->escenario->ancho;
		this->SCREEN_HEIGHT = gc->escenario->alto;
		resolucion = Resolucion::INSTANCE(SCREEN_WIDTH, SCREEN_HEIGHT);
	} else {
		setResolucion();
	}

	int spriteN = GameParser::findSprite(gc->sprites, gc->escenario->fondo);
	if (spriteN != -1)
		DIR_FONDO_PANTALLA = gc->sprites[spriteN]->path;

	inicializar();
	controllers = new HandlerPlayersControllers(gRenderer, resolucion);
}

Escenario::Escenario(int width, int height, bool isFullScreen) {
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	this->isFullScreen = isFullScreen;
	resolucion = Resolucion::INSTANCE(width, height);
	inicializar();
}

Escenario::Escenario(int fps, int width, int height) {
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
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printErrorSDL("SDL");
		inicioCorrectamente = false;
	}

	// Inicializar window
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
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
	fondoDePantalla = new Texture(gRenderer);
	if (!fondoDePantalla->loadFromFile(DIR_FONDO_PANTALLA)) {
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
	cout << "No se puede iniciarlizar " << error << endl;
	cout << "SDL Error: " << SDL_GetError() << endl;
}

void Escenario::actualizarEscenario(Posicion* pos) {
	SDL_RenderClear(gRenderer);
	fondoDePantalla->render(pos->getX(), pos->getYsdl());

	for (int i = 0; i < fondosVivibles.size(); i++) {
		fondosVivibles[i]->vivir(0, 0);
	}
	myControl->hacerVivir();
	controllers->hacerVivir();

	SDL_RenderPresent(gRenderer);
}

void Escenario::aplicarFPS(Uint32 start) {
	Uint32 dif = (SDL_GetTicks() - start);
	int time = 1000 / FRAMES_PER_SECOND;
	if (time > dif) {
		SDL_Delay(time - dif);
	}
}

void Escenario::setClient(Client* cliente) {
	this->unCliente = cliente;
}

//void Escenario::setFondosVivibles() {
//
//	for (int i = 0; i < fondosVivibles.size(); i++) {
//		int x = gc->elementos[i]->x;
//		int y = gc->elementos[i]->y;
//		int index = GameParser::findSprite(gc->sprites,gc->elementos[i]->spriteID);
//		if (index >= 0) {
//			Isla* isla = fondosVivibles[i];
//			isla->setPosicion(new Posicion(x,y));
//		}
//	}
//}

void Escenario::configurarFondosVivibles() {

	int cantRepeticiones = CANTIDAD_SCREEN_TOTAL / CANTIDAD_SCREEN;

	for (int i = 0; i < gc->elementos.size(); i++) {
		int x_gc = gc->elementos[i]->x;
		int y_gc = gc->elementos[i]->y;
		int index = GameParser::findSprite(gc->sprites,gc->elementos[i]->spriteID);

		if (index >= 0) {

			for (int j = 0; j < cantRepeticiones; j++) {
				int x = x_gc;
				int y = y_gc + (pixelesArecorrer * j);
				Posicion* p = new Posicion(x,y);
				Isla* isla = new Isla(gRenderer, p, gc->sprites[index]->path);
				fondosVivibles.push_back(isla);
			}
		}
	}

}

HandlerPlayersControllers* Escenario::getHandler() {
	return this->controllers;
}

void Escenario::configurarJugador(PlayerData* jugador) {

	Vivible* unAvion = new Avion(jugador, gRenderer, resolucion, new Posicion(SCREEN_WIDTH / 2, 100), gc->avion);
	myControl = new Controller(unAvion, gRenderer, resolucion, this->unCliente);
}

void Escenario::configurarAvionAmigo(PlayerData* playerData) {

	Vivible* avionAmigo = new Avion(playerData, gRenderer, resolucion, new Posicion(SCREEN_WIDTH / 4, 100), gc->avion);
	controllers->setPlayer((Avion*) avionAmigo);
}

void Escenario::configurarMiAvion(PlayerData* playerData) {

	Vivible* avion = new Avion(playerData, gRenderer, resolucion, new Posicion(SCREEN_WIDTH / 4, 100), gc->avion);
	myControl = new Controller(avion, gRenderer, resolucion, this->unCliente);
}

SDL_Event* Escenario::run() {

	if (!inicioCorrectamente) {
		return NULL;
	}

	int screensRecorridos = 0;

	configurarFondosVivibles();

	Posicion* posicionEscenario = new Posicion(0, 0);
	actualizarEscenario(posicionEscenario);

	Uint32 start;
	bool quit = false;
	int i = 0;

	bool isFinNivel = false;

	while (!quit) {

		start = SDL_GetTicks();
		while (SDL_PollEvent(&evento) != 0 && evento.type != SDL_MOUSEMOTION) {

			myControl->press(&evento);
			if (evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q || evento.key.keysym.sym == SDLK_r) {
				quit = true;
				break;
			}
		}

		isFinNivel = screensRecorridos >= CANTIDAD_SCREEN_TOTAL;

		if (!isFinNivel) {

			if (posicionEscenario->getY() > SCREEN_HEIGHT) {
				posicionEscenario->mover(0, VELOCIDAD_SCREEN);
			} else {
				screensRecorridos += CANTIDAD_SCREEN;
				posicionEscenario->setPosicion(0, pixelesArecorrer);
			}

			actualizarEscenario(posicionEscenario);
			aplicarFPS(start);
		}

	}

	limpiarMemoria();
	delete posicionEscenario;
	return &evento;
}

void Escenario::limpiarMemoria() {
	delete myControl;
	delete controllers;
	limpiarFondosVivibles();
	delete resolucion;
}

void Escenario::limpiarFondosVivibles() {
	if (fondosVivibles.size() > 0) {
		for (int i = 0; i < fondosVivibles.size(); i++) {
			delete fondosVivibles[i];
		}
	}
	fondosVivibles.clear();
}
