#include "Avion.h"

using namespace std;

Avion::Avion(SDL_Renderer * &renderer) {

	vistaAvion = new AvionView(renderer);
	x = 0;
	y = 0;
	anchoFondo = 600;
	largoFondo = 600;
}

Avion::~Avion(){
	delete vistaAvion;
}

int Avion::getAnchoFondo() {
	return anchoFondo;
}

int Avion::getLargoFondo() {
	return largoFondo;
}

int Avion::getAncho() {
	return vistaAvion->getAnchoVista();
}

int Avion::getLargo() {
	return vistaAvion->getLargoVista();
}

void Avion::moverEjeX(int velX) {
	x += velX;

	if ((x < 0) || (x + getAncho() > getAnchoFondo())) {
		x -= velX;
	}
}

void Avion::moverEjeY(int velY) {
	y += velY;

	if ((y < 0) || (y + getLargo() > getLargoFondo())) {
		y -= velY;
	}
}

void Avion::mover(int velX, int velY) {
	moverEjeX(velX);
	moverEjeY(velY);
}

void Avion::mostrar(){
	vistaAvion->mostrar(x,y);
}

void Avion::vivir(int velX, int velY){
	mover(velX, velY);
	mostrar();
}

