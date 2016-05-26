#include "Avion.h"

using namespace std;

Avion::Avion(PlayerData* playerData, Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, AvionConf* conf) {

	string id(playerData->name);
	string color (playerData->color);

	this->id = id;
	this->configuracion = conf;
	this->vida = 3;

	this->screen = screen;
	vistaAvion = new AvionView(renderer, color, conf->avionSpriteID);
	explosion = NULL;
	viviendo = true;

	this->posicion = posicionInicial;

	posicionAEstacionar = new Posicion(posicionInicial->getX(),posicionInicial->getY());

	anchoFondo = resolucion->getWidthScreen();
	largoFondo = resolucion->getHeightScreen();

	t = new Timer();

	estacionando = false;

	explosion = new ExplosionView("idExplosion", screen, posicion);

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

	if (vistaAvion != NULL) delete vistaAvion;
	if (explosion != NULL) delete explosion;
	delete t;
	delete posicion;
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
	if (vistaAvion != NULL)
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

bool Avion::estaEstacionando() {
	return estacionando;
}

void Avion::inicializoEstacionar() {
	estacionando = true;
	llegoPuntoDeEstacionamiento = false;
}

void Avion::mostrarEstacionar(int frame){
	vistaAvion->mostrarEstacionar(posicion->getX(),posicion->getYsdl(),frame); //estacionar
}

void Avion::estacionar() {
	int velocidadEstacionado = 1; //TODO: habra que sacarlo del XML??
	int frame = 0;

	//Funcion para automaticar el estacionamiento

	if (!llegoPuntoDeEstacionamiento){
		if (posicion->getX() != posicionAEstacionar->getX()) {
			if (posicion->getX() > posicionAEstacionar->getX()) {
				mover(-velocidadStandard, 0);
			} else {
				mover(velocidadStandard, 0);
			}
		}
		if (posicion->getY() != posicionAEstacionar->getY()) {
			if (posicion->getY() > posicionAEstacionar->getY()) {
				mover(0, -velocidadStandard);
			} else {
				mover(0, velocidadStandard);
			}
		}
		llegoPuntoDeEstacionamiento = (posicion->getX() == posicionAEstacionar->getX() && posicion->getY() == posicionAEstacionar->getY());

	} else {
		//TODO: Aqui deberia enviar al servidor de que el avion ha teriminado de estacionar

		if (!realizandoVueltereta){ //Esto porque la idea es que cuando empieza otro nivel haga la vueltereta
			mover(0, velocidadEstacionado);
		} else {
			estacionando = false;
		}
	}

	mostrarEstacionar(frame);
}

void Avion::vivir(int velX, int velY){

	if (tieneHP()) {
		if ((velX != 0 || velY != 0) && !viviendo) {
			this->viviendo = true;
			vistaAvion->conectar();
		}
		if (estacionando) {
			estacionar();
		} else {
			if (!realizandoVueltereta) {
				mover(velX, velY);
				mostrar(velX);
			} else {
				realizoVueltereta();
			}
		}
	} else {
		if (vistaAvion != NULL) {
			delete vistaAvion;
			vistaAvion = NULL;
		}

		if (!explosion->exploto()) {
			posicion->mover(-1, -3);
			explosion->explotar(posicion);
		}
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
	if (vistaAvion != NULL)
	vistaAvion->desconectar();
}

void Avion::setAmetralladora(){
	if (vistaAvion != NULL)
	vistaAvion->ametralladora();
}

void Avion::recibirMisil(Misil* misil) {
	if (tieneHP()) {
		this->vida -= misil->getDano();
		cout << "La vida actual es " << this->vida << endl;
	}
}

bool Avion::tieneHP() {
	return (this->vida > 0);
}

void Avion::setHP(int hp) {
	this->vida = hp;
}

Posicion* Avion::getPosicion() {
	return this->posicion;
}

void Avion::despegar() {
	estacionando = false;
}

bool Avion::hit(int x, int y) {
	return (x >= posicion->getX()) &&
	       (x <= posicion->getX() + vistaAvion->getAnchoVista()) &&
	       (y >= posicion->getY()) &&
	       (y <= posicion->getY() + vistaAvion->getLargoVista());
}
