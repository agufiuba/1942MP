#include "Enemy.h"

using namespace std;

Enemy::Enemy(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, AvionConf* conf, Avion* avionApuntado) {

	this->id = id;
	this->configuracion = conf;
	this->vida = 1;

	this->screen = screen;
	vistaAvion = new AvionView(renderer, "verde", conf->avionSpriteID);
	explosion = NULL;
	viviendo = true;

	this->posicion = posicionInicial;

	anchoFondo = resolucion->getWidthScreen();
	largoFondo = resolucion->getHeightScreen();
	this->resolucion = resolucion;

	angleX = 0;
	angleY = -1;

	t = new Timer();

    flota = -1;
    posFlota = 0;
	explosion = new ExplosionView("idExplosion", screen, posicion);

	firsttime = true;
	time_t tp = time(0);
	tm = localtime(&tp);
	nFlota = chrono::system_clock::now();

	misilConf = new MisilConf();
	strcpy(misilConf->disparosSpriteID,conf->disparosSpriteID);
	misilConf->velocidadDisparos = conf->velocidadDisparos + conf->velocidadDesplazamiento;
	controlDeMisiles = new ControllerMissilesEnemy(misilConf, renderer);
	this->avionApuntado = avionApuntado;
	contador = 0;
	tiempoEntreDisparo = 20;
}

Enemy::~Enemy(){
	if (!explosion->exploto()) {
	   posicion->mover(-1, -3);
	   explosion->explotar(posicion);
	}

	if (vistaAvion != NULL) delete vistaAvion;
	if (explosion != NULL) delete explosion;
	delete t;
	delete posicion;
	delete controlDeMisiles;
}

string Enemy::getId() {
	return this->id;
}

void Enemy::setVelocidadStandard(int vel){
	velocidadStandard = vel;
}

int Enemy::getAnchoFondo() {
	return anchoFondo;
}

int Enemy::getLargoFondo() {
	return largoFondo;
}

int Enemy::getAncho() {
	if (this->vistaAvion != NULL){
		return vistaAvion->getAnchoVista();
	} else {
		return 0;
	}
}

int Enemy::getLargo() {
	if (this->vistaAvion != NULL){
		return vistaAvion->getLargoVista();
	} else {
		return 0;
	}
}

AvionConf* Enemy::getConfiguracion() {
	return this->configuracion;
}

void Enemy::moverEjeX(int velX) {
	posicion->mover(velX,0);
	if ((posicion->getX() < 0) || (posicion->getX() + getAncho() > getAnchoFondo())) {
		posicion->mover(-velX,0);
	}
}

void Enemy::moverEjeY(int velY) {
	posicion->mover(0,velY);
	if ((posicion->getY() - getLargo() < 0) || (posicion->getY() > getLargoFondo())) {
		posicion->mover(0,-velY);
	}
}

void Enemy::disparar() {
	if (contador >= tiempoEntreDisparo){
		controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + 12,this->getY(), resolucion, misilConf, avionApuntado->getX(), avionApuntado->getY());
		contador = 0;
	}
		contador ++;
}

void Enemy::mostrarDisparo() {
	controlDeMisiles->hacerVivir();
}

void Enemy::mover(int velX, int velY) {
	moverEjeX(velX);
	moverEjeY(velY);
}

void Enemy::mostrar(int velX, int velY){
	if (vistaAvion != NULL)
	vistaAvion->mostrarRotado(posicion->getX(),posicion->getYsdl(),velX, velY);
}

void Enemy::vivirRandom(){
	if (tieneHP()) {
//		if (!viviendo) {
//			this->viviendo = true;
//			vistaAvion->conectar();
//		}
		disparar();
		mostrarDisparo();
		moverRandom();
		mostrar(angleX, angleY);
	} else {
		if (vistaAvion != NULL) {
			delete vistaAvion;
			vistaAvion = NULL;
		}

		if (!explosion->exploto()) {
			posicion->mover(-1, -3);
			explosion->explotar(posicion);
		} else {
			this->viviendo = false;
		}
	}

}

void Enemy::vivirFlota(){
	if (tieneHP()) {
//		if (!viviendo) {
//			this->viviendo = true;
//			vistaAvion->conectar();
//		}
		disparar();
		mostrarDisparo();
		moverFlota();
		mostrar(angleX, angleY);
	} else {
		if (vistaAvion != NULL) {
			delete vistaAvion;
			vistaAvion = NULL;
		}

		if (!explosion->exploto()) {
			posicion->mover(-1, -3);
			explosion->explotar(posicion);
		} else {
			this->viviendo = false;
		}
	}

}

void Enemy::vivir(int velX, int velY){
	if (tieneHP()) {
//		if ((velX != 0 || velY != 0) && !viviendo) {
//			this->viviendo = true;
//			vistaAvion->conectar();
//		}
//		if (!realizandoVueltereta) {
			mover(velX, velY);
			mostrar(angleX, angleY);
//		} else {
//			realizoVueltereta();
//		}
	} else {
		if (vistaAvion != NULL) {
			delete vistaAvion;
			vistaAvion = NULL;
		}

		if (!explosion->exploto()) {
			posicion->mover(-1, -3);
			explosion->explotar(posicion);
		} else {
			this->viviendo = false;
		}
	}

}

int Enemy::getX(){
	return posicion->getX();
}


int Enemy::getY(){
	return posicion->getY();
}

bool Enemy::aunVive(){
	return viviendo;
}

void Enemy::recibirMisil(Misil* misil) {
	if (tieneHP()) {
		this->vida -= misil->getDano();
		cout << "La vida actual es " << this->vida << endl;
	}
	if(!tieneHP()){
		viviendo = false;
	}
}

bool Enemy::tieneHP() {
	return (this->vida > 0);
}

void Enemy::setHP(int hp) {
	this->vida = hp;
}

Posicion* Enemy::getPosicion() {
	return this->posicion;
}

bool Enemy::hit(int x, int y) {
	return (x >= posicion->getX()) &&
	       (x <= posicion->getX() + vistaAvion->getAnchoVista()) &&
	       (y >= posicion->getY()) &&
	       (y <= posicion->getY() + vistaAvion->getLargoVista());
}

void Enemy::moverRandom() {
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

	mover(x, y);
}

void Enemy::moverFlota() {
	auto now = chrono::system_clock::now();
	chrono::duration<double> duration = now - nFlota;
	float delay = posFlota / 3.5;
	if (duration.count() > delay) {
		if(duration.count() < 1.5 + delay) {
			mover(5, 0);
			angleX = 4;
			angleY = 0;
		}
		else {
			if(duration.count() < 2 + delay) {
				mover(3, 3);
				angleX = 2;
				angleY = 2;
			}
			else {
				if(duration.count() < 2.5 + delay) {
					mover(-3, 3);
					angleX = -2;
					angleY = 2;
				}
				else {
					if(duration.count() < 3.5 + delay) {
						mover(-5, 0);
						angleX = -2;
						angleY = 0;
					}
					else {
						if(duration.count() < 4 + delay) {
							mover(-3, -3);
							angleX = -1;
							angleY = -1;
						}
						else {
							if(duration.count() < 4.5 + delay) {
								mover(3, -3);
								angleX = 1;
								angleY = -1;
							}
							else {
								mover(5, 0);
								angleX = 1;
								angleY = 0;
							}
						}
					}
				}
			}
		}
	}
}

void Enemy::morir(){
	this->setHP(0);
//	viviendo = false;
}

ControllerMissilesEnemy* Enemy::getControllerMissiles(){
	return this->controlDeMisiles;
}
