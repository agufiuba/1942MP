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

void Avion::mostrar(int velX){
	vistaAvion->mostrar(x,y,velX);
}

void Avion::realizoVueltereta() {
	int tiempoIda = 2000;
	int tiempoVuelta = tiempoIda;
	int tiempoMuerto = 500;

	if (t->tiempoActual() < tiempoIda) {
		mover(0, velocidadEnVueltereta);

	} else {
		if (t->tiempoActual() < tiempoIda + tiempoMuerto) {

		} else {
			if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta) {
				mover(0, -velocidadEnVueltereta);

			} else {
				if (t->tiempoActual() < tiempoIda + tiempoMuerto*2 + tiempoVuelta) {

				} else {
					if (t->tiempoActual() < tiempoIda + tiempoMuerto*2 + tiempoVuelta + tiempoIda/4) {
						mover(0, velocidadEnVueltereta);

					} else {
						realizandoVueltereta = false;
						t->parar();
					}
				}
			}
		}
	}
	mostrar(0); //Para que el usuario no pueda doblar en este periodo
}

void Avion::vivir(int velX, int velY){
	if (!realizandoVueltereta){
		mover(velX, velY);
		mostrar(velX);
	} else {
		realizoVueltereta();
	}
}

