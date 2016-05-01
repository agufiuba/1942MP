#include "Avion.h"

using namespace std;

Avion::Avion(SDL_Renderer * &renderer) {
	vistaAvion = new AvionView(renderer);

	x = 250;
	y = 450;

	anchoFondo = 600; //TODO: hay que cambiarlo para que pueda conseguirlo desde el escenario
	largoFondo = 600;

	t = new Timer();

	inicializoVueltereta();
}

void Avion::inicializoVueltereta() {
	velocidadEnVueltereta = -(getLargo() / 4);
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

void Avion::mostrarVueltereta(int frame){
	vistaAvion->mostrarVueltereta(x,y,frame);
}

void Avion::realizoVueltereta() {
	int tiempoIda = 1800;
	int tiempoVuelta = tiempoIda;
	int tiempoMuerto = 300;
	int frame;

	if (t->tiempoActual() < tiempoIda) {
		frame = 0;
		mover(0, velocidadEnVueltereta);

	} else {
		if (t->tiempoActual() < tiempoIda + tiempoMuerto / 2) {
			frame = 1;

		} else {
			if (t->tiempoActual() < tiempoIda + tiempoMuerto) {
				frame = 2;

			} else {
				if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta) {
					frame = 3;
					mover(0, -velocidadEnVueltereta);

				} else {
					if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta + tiempoMuerto / 2) {
						frame = 4;

					} else {
						if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta + tiempoMuerto) {
							frame = 5;

						} else {
							if (t->tiempoActual() < tiempoIda + tiempoMuerto * 2 + tiempoVuelta + tiempoIda / 4) {
								frame = 0;
								mover(0, velocidadEnVueltereta);

							} else {
								realizandoVueltereta = false;
								t->parar();
							}
						}
					}
				}
			}
		}
	}
	mostrarVueltereta(frame);
}


void Avion::vivir(int velX, int velY){
	if (!realizandoVueltereta){
		mover(velX, velY);
		mostrar(velX);
	} else {
		realizoVueltereta();
	}
}

