/*
 * ServerAvionEnemigoRandom.cpp
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoRandom.h"

ServerAvionEnemigoRandom::ServerAvionEnemigoRandom(Posicion* posicionInicial, Avion* avionApuntado) {

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

	this->avionApuntado = avionApuntado;
	contador = 0;
	tiempoEntreDisparo = 20;
}

ServerAvionEnemigoRandom::~ServerAvionEnemigoRandom(){
	delete t;
	delete posicion;
}

string ServerAvionEnemigoRandom::getId() {
	return this->id;
}

int ServerAvionEnemigoRandom::getX(){
	return posicion->getX();
}

int ServerAvionEnemigoRandom::getY(){
	return posicion->getY();
}

int ServerAvionEnemigoRandom::getAnchoFondo() {
	return anchoFondo;
}

int ServerAvionEnemigoRandom::getLargoFondo() {
	return largoFondo;
}

int ServerAvionEnemigoRandom::getLargo() {
	return largoVista;
}

int ServerAvionEnemigoRandom::getAncho() {
	return anchoVista;
}

void ServerAvionEnemigoRandom::disparar() {
	if (contador >= tiempoEntreDisparo){
		//TODO: hay que agregar el envio de que dispare
		contador = 0;
	}
		contador ++;
}

bool ServerAvionEnemigoRandom::aunVive(){
	return viviendo;
}

void ServerAvionEnemigoRandom::recibirMisil(Misil* misil) {
	if (tieneHP()) {
		this->vida -= misil->getDano();
		cout << "La vida actual es " << this->vida << endl;
	}
	if(!tieneHP()){
		viviendo = false;
	}
}

bool ServerAvionEnemigoRandom::tieneHP() {
	return (this->vida > 0);
}

void ServerAvionEnemigoRandom::setHP(int hp) {
	this->vida = hp;
}

void ServerAvionEnemigoRandom::morir(){
	this->setHP(0);
}

Posicion* ServerAvionEnemigoRandom::getPosicion() {
	return this->posicion;
}

void ServerAvionEnemigoRandom::mover() {
	int x, y;
	time_t temp = time(0);
	localtime(&temp);

	if ((tm->tm_sec - secsRandom) < 1 && !firsttime) {
		x = angleX;
		y = angleY;
	}
	else {
		srand(time(NULL));
		x = rand() % 11 - 5;
		y = rand() % 11 - 5;

		// centrar +/-
		x -= (posicion->getX() - anchoFondo / 2) / 120;
		y -= (posicion->getY() - largoFondo / 2) / 150;
		firsttime = false;
		angleX = x;
		angleY = y;
		secsRandom = tm->tm_sec;
	}

	moverEjeX(x);
	moverEjeY(y);
}

void ServerAvionEnemigoRandom::moverEjeX(int velX) {
	posicion->mover(velX,0);
	if ((posicion->getX() < 0) || (posicion->getX() + anchoVista > anchoFondo)) {
		posicion->mover(-velX,0);
	}
}

void ServerAvionEnemigoRandom::moverEjeY(int velY) {
	posicion->mover(0,velY);
	if ((posicion->getY() - largoVista < 0) || (posicion->getY() > largoFondo)) {
		posicion->mover(0,-velY);
	}
}
