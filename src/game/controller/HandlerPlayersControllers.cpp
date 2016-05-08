/*
 * HandlerPlayersControllers.cpp
 *
 *  Created on: 07/05/2016
 *      Author: gonzalo
 */

#include "HandlerPlayersControllers.h"

HandlerPlayersControllers::HandlerPlayersControllers(SDL_Renderer* &renderer) {
	this->renderer = renderer;
}

HandlerPlayersControllers::~HandlerPlayersControllers() {
	for (map<string, IController*>::iterator it = this->mapaControllers.begin(); it != this->mapaControllers.end(); ++it) {
		delete it->second;
	}
}

void HandlerPlayersControllers::setPlayer(Avion* player) {
	IController* controller = new PlayersController(player, renderer);
	this->mapaControllers[player->getId()] = controller;
}

void HandlerPlayersControllers::mover(string id, char evento) {

	IController* controller = mapaControllers[id];
	SDL_Event* eventoSdl;
	SDL_Keycode code;

	switch(evento) {
		case 'R': code = SDLK_d; break;
		case 'L': code = SDLK_a; break;
		case 'U': code = SDLK_w; break;
		case 'D': code = SDLK_s; break;
	}

	eventoSdl->key.keysym.sym = code;
	controller->press(eventoSdl);
}

void HandlerPlayersControllers::hacerVivir() {
	for (map<string, IController*>::iterator it = this->mapaControllers.begin(); it != this->mapaControllers.end(); ++it) {
		it->second->hacerVivir();
	}
}


