/*
 * ServerAvionEnemigo.cpp
 *
 *  Created on: 17 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigo.h"

ServerAvionEnemigo::ServerAvionEnemigo( int id, Posicion* posicionInicial ) {
	this->id = id;
	this->vida = 1;

	anchoVista = 70; //TODO: hardcodeado por ahora, porque hay que conseguirlo desde el cliente
	largoVista = 60;

	viviendo = true;

	this->posicion = posicionInicial;

	anchoFondo = 1024; //TODO: hardcodeado por ahora, porque hay que conseguirlo desde el cliente
	largoFondo = 700;

	angleX = 0;
	angleY = -1;

	t = new Timer();

	firsttime = true;
	time_t tp = time(0);
	tm = localtime(&tp);
	nFlota = chrono::system_clock::now();

	contador = 0;
	tiempoEntreDisparo = 20;
	this->active = false;
}

ServerAvionEnemigo::~ServerAvionEnemigo(){
	delete t;
	delete posicion;
}

char ServerAvionEnemigo::getType() {
	return this->type;
}

int ServerAvionEnemigo::getId() {
	return this->id;
}

int ServerAvionEnemigo::getX(){
	return posicion->getX();
}

int ServerAvionEnemigo::getY(){
	return posicion->getY();
}

int ServerAvionEnemigo::getAnchoFondo() {
	return anchoFondo;
}

int ServerAvionEnemigo::getLargoFondo() {
	return largoFondo;
}

int ServerAvionEnemigo::getLargo() {
	return largoVista;
}

int ServerAvionEnemigo::getAncho() {
	return anchoVista;
}

void ServerAvionEnemigo::disparar() {
	if (contador >= tiempoEntreDisparo){
		//TODO: hay que agregar el envio de que dispare
		contador = 0;
	}
		contador ++;
}

bool ServerAvionEnemigo::aunVive(){
	return viviendo;
}

void ServerAvionEnemigo::recibirMisil(Misil* misil) {
	if (tieneHP()) {
		this->vida -= misil->getDano();
		cout << "La vida actual es " << this->vida << endl;
	}
	if(!tieneHP()){
		viviendo = false;
	}
}

void ServerAvionEnemigo::bajarHP() {
  if ( this->tieneHP() ) {
    this->vida--;
    cout << "ENEMY " << to_string( this->id ) << " HP = " << this->vida << endl;
  }
  if ( !this->tieneHP() ) {
    this->viviendo = false;
  }
}

bool ServerAvionEnemigo::tieneHP() {
	return (this->vida > 0);
}

void ServerAvionEnemigo::setHP(int hp) {
	this->vida = hp;
}

void ServerAvionEnemigo::morir(){
	this->setHP(0);
}

Posicion* ServerAvionEnemigo::getPosicion() {
	return this->posicion;
}

void ServerAvionEnemigo::moverEjeX(int velX) {
	posicion->mover(velX,0);
	if ((posicion->getX() < 0) || (posicion->getX() + anchoVista > anchoFondo)) {
		posicion->mover(-velX,0);
	}
}

void ServerAvionEnemigo::moverEjeY(int velY) {
	posicion->mover(0,velY);
	if ((posicion->getY() - largoVista < 0) || (posicion->getY() > largoFondo)) {
		posicion->mover(0,-velY);
	}
}

EnemyData* ServerAvionEnemigo::vivir() {}

bool ServerAvionEnemigo::isActive() {
  return this->active;
}

void ServerAvionEnemigo::activate() {
  this->active = true;
}

void ServerAvionEnemigo::updatePosition( int x, int y ) {
  this->posicion->setPosicion( x, y ); 
}

int ServerAvionEnemigo::getHitScore() { return 0; }

int ServerAvionEnemigo::getKillScore() { return 0; }
