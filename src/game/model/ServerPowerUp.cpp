/*
 * ServerPowerUp.cpp
 *
 *  Created on: 21 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerPowerUp.h"


ServerPowerUp::ServerPowerUp( char type, Posicion* posicionInicial ) {
	this->type = type;
	this->vida = 1;

	anchoVista = 70; //TODO: hardcodeado por ahora, porque hay que conseguirlo desde el cliente
	largoVista = 60;

	viviendo = true;

	this->posicion = posicionInicial;

	anchoFondo = 1024; //TODO: hardcodeado por ahora, porque hay que conseguirlo desde el cliente
	largoFondo = 700;

	this->active = false;
	this->apareceEn = 0;
}

int ServerPowerUp::getApareceEn(){
	return this->apareceEn;
}
void ServerPowerUp::setApareceEn(int apareceEn){
	this->apareceEn = apareceEn;
}

ServerPowerUp::~ServerPowerUp(){
	delete posicion;
}

char ServerPowerUp::getType() {
	return this->type;
}

int ServerPowerUp::getX(){
	return posicion->getX();
}

int ServerPowerUp::getY(){
	return posicion->getY();
}

int ServerPowerUp::getAnchoFondo() {
	return anchoFondo;
}

int ServerPowerUp::getLargoFondo() {
	return largoFondo;
}

int ServerPowerUp::getLargo() {
	return largoVista;
}

int ServerPowerUp::getAncho() {
	return anchoVista;
}

bool ServerPowerUp::aunVive(){
	return viviendo;
}

bool ServerPowerUp::tieneHP() {
	return (this->vida > 0);
}

void ServerPowerUp::setHP(int hp) {
	this->vida = hp;
}

void ServerPowerUp::morir(){
	this->setHP(0);
}

Posicion* ServerPowerUp::getPosicion() {
	return this->posicion;
}

void ServerPowerUp::moverEjeX(int velX) {
	posicion->mover(velX,0);
	if ((posicion->getX() < 0) || (posicion->getX() + anchoVista > anchoFondo)) {
		posicion->mover(-velX,0);
	}
}

void ServerPowerUp::moverEjeY(int velY) {
	posicion->mover(0,velY);
	if ((posicion->getY() - largoVista < 0) || (posicion->getY() > largoFondo)) {
		//TODO: Aca estaria fuera del escenario y deberia morir
		this->viviendo = false;
	}
}

bool ServerPowerUp::isActive() {
  return this->active;
}

void ServerPowerUp::activate() {
  this->active = true;
}

void ServerPowerUp::updatePosition( int x, int y ) {
  this->posicion->setPosicion( x, y );
}
