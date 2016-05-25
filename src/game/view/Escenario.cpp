/*
 * Escenario.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "Escenario.h"
using namespace std;

Escenario::Escenario(GameConf* configuracion, XM_SDL* sdl) {
	this->gc = configuracion;
	this->sdl = sdl;

	this->SCREEN_WIDTH = gc->escenario->ancho;
	this->SCREEN_HEIGHT = gc->escenario->alto;
	this->LONGITUD_NIVEL = gc->escenario->longitudNivel;
	this->CANTIDAD_NIVELES = gc->escenario->cantidadNiveles;
	resolucion = Resolucion::INSTANCE(SCREEN_WIDTH, SCREEN_HEIGHT);

	int spriteN = GameParser::findSprite(gc->sprites, gc->escenario->fondo);
	if (spriteN != -1) {

		DIR_FONDO_PANTALLA = gc->sprites[spriteN]->path;
	}

	gRenderer = sdl->getRenderer();
	escenarioScreen = new Screen(this->sdl);
	escenarioScreen->loadTexture("agua", "fondos/" + DIR_FONDO_PANTALLA);

	controllers = new HandlerPlayersControllers(gRenderer, resolucion);
}

Escenario::~Escenario() {
	resolucion->~Resolucion();
	delete escenarioScreen;
	delete myControl;
	delete controllers;
	limpiarFondosVivibles();
}

void Escenario::actualizarEscenario(Posicion* pos) {

	escenarioScreen->renderTexture("agua", pos->getX(), pos->getYsdl());

	for (int i = 0; i < fondosVivibles.size(); i++) {
		fondosVivibles[i]->vivir();
	}

	controllers->hacerVivir();
	myControl->hacerVivir();
	hPowerUp->hacerVivir();

	SDL_RenderPresent(gRenderer);
	// set new offset on client
	this->unCliente->setStageOffset(pixelesRecorridos);
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

void Escenario::configurarFondosVivibles() {

	int cantidadRepeticionesPorNivel = 1;
	if (LONGITUD_NIVEL > LIMITE_IMAGEN) {
		cantidadRepeticionesPorNivel = LONGITUD_NIVEL / LIMITE_IMAGEN;
	}

	int cantRepeticiones = (CANTIDAD_NIVELES * cantidadRepeticionesPorNivel) + 1;

	for (int i = 0; i < gc->elementos.size(); i++) {
		int x_gc = gc->elementos[i]->x;
		int y_gc = gc->elementos[i]->y;
		int index = GameParser::findSprite(gc->sprites,gc->elementos[i]->spriteID);

		if (index >= 0) {

			for (int j = 0; j < cantRepeticiones; j++) {
				int x = x_gc;
				int y = y_gc + (LIMITE_IMAGEN * j);
				Posicion* p = new Posicion(x, y);
				string jString  = to_string(j);
				Isla* isla = new Isla(jString, p, gc->sprites[index], escenarioScreen);
				fondosVivibles.push_back(isla);
			}
		}
	}
}

HandlerPlayersControllers* Escenario::getHandler() {
	return this->controllers;
}

void Escenario::configurarAvionAmigo(PlayerData* playerData) {
	Vivible* avionAmigo = new Avion(playerData, gRenderer, resolucion, new Posicion(playerData->x, playerData->y), gc->avion);
	controllers->setPlayer((Avion*) avionAmigo);
}

void Escenario::configurarMiAvion(PlayerData* playerData) {
	Vivible* avion = new Avion(playerData, gRenderer, resolucion, new Posicion(playerData->x, playerData->y), gc->avion);
	myControl = new Controller(avion, gRenderer, resolucion, this->unCliente);
}

void Escenario::setFondosVivibles(int x, int y) {

	for (int i = 0; i < fondosVivibles.size(); i++) {
		fondosVivibles[i]->vivir(x, y);
	}

}

SDL_Event* Escenario::run() {

	pixelesRecorridos = 0;
	configurarFondosVivibles();
	Posicion* posicionEscenario = new Posicion(0, 0);

	//Reinicia mediante R no entra a buscar el offset, sino si (caso: salio por Q y vuelve a ingresar)
	if (!this->unCliente->reinicia) {

		int offset = this->unCliente->getStageOffset();
		if (offset != 0) {
			pixelesRecorridos = offset + desfasajeConexion;
			setFondosVivibles(0, pixelesRecorridos);
		}
	}

	actualizarEscenario(posicionEscenario);
	Uint32 start;
	bool quit = false;

	for (int numeroNivel = 1; numeroNivel < (CANTIDAD_NIVELES + 1); numeroNivel++) {

		while (!quit && this->unCliente->isConnected()) {

			start = SDL_GetTicks();

			if (this->unCliente->reset) {
				SDL_Event* eventReset = new SDL_Event();
				eventReset->key.keysym.sym = SDLK_r;
				this->unCliente->setStageOffset(0);
				return eventReset;
			}

			while (this->sdl->nextEvent(&evento) && evento.type != SDL_MOUSEMOTION) {

				myControl->press(&evento);

				if( evento.type == SDL_KEYDOWN ) {
					if (evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q || evento.key.keysym.sym == SDLK_r || this->unCliente->reset) {
						quit = true;

						PlayerData* p = new PlayerData();

						strcpy(p->name, (myControl->getVivible())->getId().c_str());
						p->x = myControl->getVivible()->getX();
						p->y = myControl->getVivible()->getY();

						while (!this->unCliente->sendDataPosicion(p))
							;
						usleep(100);

						break;
					}
				}

			}

//			isFinNivel = pixelesRecorridos >= LONGITUD_NIVEL * numeroNivel;

			if (isFinNivel(numeroNivel)) {

//				TODO: Aca deberia ir la inscripcion de fin de nivel
				cout << "SE TERMINO EL NIVEL " << numeroNivel << endl;
				usleep(5000000);
				break;
			} else {

				pixelesRecorridos -= VELOCIDAD_SCREEN;
				if (posicionEscenario->getY() > SCREEN_HEIGHT) {
					posicionEscenario->mover(0, VELOCIDAD_SCREEN);
				} else {
					posicionEscenario->setPosicion(0, LIMITE_IMAGEN);
				}

				actualizarEscenario(posicionEscenario);
				aplicarFPS(start);
			}
		}

	}

	delete posicionEscenario;
	return &evento;
}

bool Escenario::isFinNivel(int numeroNivel) {
	return pixelesRecorridos >= LONGITUD_NIVEL * numeroNivel;
}

void Escenario::limpiarFondosVivibles() {

	if (fondosVivibles.size() > 0) {
		for (int i = 0; i < fondosVivibles.size(); i++) {
			delete fondosVivibles[i];
		}
	}
	fondosVivibles.clear();
}

