#include "Avion.h"

using namespace std;

Avion::Avion(PlayerData* playerData, SDL_Renderer * &renderer, Resolucion* &resolucion, Posicion* posicionInicial, AvionConf* conf) {

	string id(playerData->name);
	string color (playerData->color);

	this->id = id;
	this->configuracion = conf;

	vistaAvion = new AvionView(renderer, color, conf->avionSpriteID);
	viviendo = true;

	this->posicion = posicionInicial;

	anchoFondo = resolucion->getWidthScreen();
	largoFondo = resolucion->getHeightScreen();

	t = new Timer();

/*	inicializoVueltereta();*/
}

void Avion::inicializoVueltereta() {
	realizandoVueltereta = true;
	t->correr();
}

bool Avion::haciendoVueltereta(){
	return this->realizandoVueltereta;
}

Avion::~Avion(){
	delete vistaAvion;
	delete t;
}

string Avion::getId() {
	return this->id;
}

void Avion::setVelocidadStandard(int vel){
	velocidadStandard = vel;
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

AvionConf* Avion::getConfiguracion() {
	return this->configuracion;
}

void Avion::moverEjeX(int velX) {
	posicion->mover(velX,0);
//	cout << "El ancho avion es: " << getAncho() << endl;
//	cout << "El ancho fondo es: " << getAnchoFondo() << endl;
//	cout << "El largo fondo es: " << getLargoFondo() << endl;
	if ((posicion->getX() < 0) || (posicion->getX() + getAncho() > getAnchoFondo())) {
		posicion->mover(-velX,0);
	}
}

void Avion::moverEjeY(int velY) {
	posicion->mover(0,velY);
	//cout << "El largo fondo es: " << getLargoFondo() << endl;
	if ((posicion->getY() - getLargo() < 0) || (posicion->getY() > getLargoFondo())) {
		posicion->mover(0,-velY);
	}
}

void Avion::mover(int velX, int velY) {
	moverEjeX(velX);
	moverEjeY(velY);
}

void Avion::mostrar(int velX){
	vistaAvion->mostrar(posicion->getX(),posicion->getYsdl(),velX);
}

void Avion::mostrarVueltereta(int frame){
	vistaAvion->mostrarVueltereta(posicion->getX(),posicion->getYsdl(),frame);
}

void Avion::realizoVueltereta() {
	int tiempoIda = 1600; //recomendado >> 1600
	int tiempoVuelta = tiempoIda;
	int tiempoMuerto = 400; //recomendado >> 400
	int frame = 0;

	if (t->tiempoActual() < tiempoIda) {
		frame = 0;
		mover(0, velocidadStandard);

	} else {
		if (t->tiempoActual() < tiempoIda + tiempoMuerto / 2) {
			frame = 1;

		} else {
			if (t->tiempoActual() < tiempoIda + tiempoMuerto) {
				frame = 2;

			} else {
				if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta) {
					frame = 3;
					mover(0, -velocidadStandard);

				} else {
					if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta + tiempoMuerto / 2) {
						frame = 4;

					} else {
						if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoVuelta + tiempoMuerto) {
							frame = 5;

						} else {
							if (t->tiempoActual() < tiempoIda + tiempoMuerto * 2 + tiempoVuelta + tiempoIda / 4) {
								frame = 0;
								mover(0, velocidadStandard);

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
	if ((velX != 0 || velY != 0) && !viviendo) {
		this->viviendo = true;
		vistaAvion->conectar();
	}

	if (!realizandoVueltereta){
		mover(velX, velY);
		mostrar(velX);
	} else {
		realizoVueltereta();
	}
}

int Avion::getX(){
	return posicion->getX();
}


int Avion::getY(){
	return posicion->getY();
}

bool Avion::aunVive(){
	return viviendo;
}

void Avion::desconectar(){
	this->viviendo = false;
	vistaAvion->desconectar();
}
