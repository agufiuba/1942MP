#include "Avion.h"

using namespace std;

Avion::Avion(SDL_Renderer * &renderer) {
	vistaAvion = new AvionView(renderer);

	x = 220;
	y = 480;

	anchoFondo = 600; //TODO: hay que cambiarlo para que pueda conseguirlo desde el escenario
	largoFondo = 600;

	t = new Timer();

	inicializoVueltereta();
}

void Avion::inicializoVueltereta() {
	velocidadEnVueltereta = -(getLargo() / 8);
	realizandoVueltereta = true;
	t->correr();
}

Avion::~Avion(){
	delete vistaAvion;
	delete t;
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

void Avion::realizoVueltereta(){
	int tiempoDeDesplazamiento = 2000;

	if (t->tiempoActual() < tiempoDeDesplazamiento){
		mover(0, velocidadEnVueltereta);
	} else {
		if (t->tiempoActual() < tiempoDeDesplazamiento*2){
			mover(0, -velocidadEnVueltereta);
		} else {
			if (t->tiempoActual() < ((tiempoDeDesplazamiento*2) + 500)){
				mover(0, velocidadEnVueltereta);
			} else {
				realizandoVueltereta = false;
				t->parar();
			}
		}
	}
}

void Avion::vivir(int velX, int velY){
	if (!realizandoVueltereta){
		mover(velX, velY);
	} else {
		realizoVueltereta();
	}
	mostrar();
}

