/*
 * EnemyGrande.cpp
 *
 *  Created on: 18 de jun. de 2016
 *      Author: ramon
 */

#include "EnemyGrande.h"

EnemyGrande::EnemyGrande(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, GameConf* conf)
:Enemy(screen, renderer, resolucion, posicionInicial, conf) {
		this->setHP(10);
		this->vistaAvion->setAnchoVista(250);
		this->vistaAvion->setLargoVista(170);
		this->vistaAvion->cargarClips();
		this->vistaAvion->cargarImagenDelAvion("AvionSprites_grande.bmp");
		this->posXBala = 125;
		this->tiempoEntreDisparo = 50;
		this->tipo = 'g';
}

EnemyGrande::~EnemyGrande() {
//	cout << "Muere Grande"  << endl;
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

void EnemyGrande::disparar() {
//	cout<<"Disparo Grande"<<endl;
	bool estaEnMovimiento = !(this->velX == 0 && this->velY == 0);
	if(posicion->getY() + getLargo() > 0 && posicion->getY() < getLargoFondo() &&
		posicion->getX() + getAncho() > 0 && posicion->getX() < getAnchoFondo()) {
	if (contador >= tiempoEntreDisparo && !estaEnMovimiento){
//		int posBalaInicial = 100;
//		for (int var = 0; var < 4; ++var) {
//			cout<<"Disparo "<<posBalaInicial <<endl;
//			controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + this->posXBala,this->getY()-this->getLargo()/2, resolucion, misilConf, 0, 0);
			controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + this->posXBala,this->getY()-this->getLargo()/2, resolucion, misilConf, 100, 0);
			controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + this->posXBala,this->getY()-this->getLargo()/2, resolucion, misilConf, 300, 0);
			controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + this->posXBala,this->getY()-this->getLargo()/2, resolucion, misilConf, 500, 0);
			controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + this->posXBala,this->getY()-this->getLargo()/2, resolucion, misilConf, 700, 0);
			controlDeMisiles->crearNuevoMisilEnPosicion(this->getX() + this->posXBala,this->getY()-this->getLargo()/2, resolucion, misilConf, 900, 0);

//			posBalaInicial = posBalaInicial+200;
//		}
		contador = 0;
	}
		contador ++;
	}
}

int EnemyGrande::getLargo() {

	if (this->vistaAvion != NULL){
		return vistaAvion->getLargoVista() - 60;
	} else {
		return 0;
	}
}
