#ifndef SRC_MODEL_Enemy_H_
#define SRC_MODEL_Enemy_H_

#include <iostream>
#include <ctime>
#include <chrono>
#include "../view/EnemyView.h"
#include "Vivible.h"

#include "../controller/Timer.h"
#include "Resolucion.h"
#include "../model/Posicion.h"
#include "../../libs/data/player_data.h"
#include "../../xml/conf/GameConf.h"
#include <string>
#include "Misil.h"
#include "../view/ExplosionView.h"
#include "../view/Screen.h"
#include "../controller/ControllerMissilesEnemy.h"

class Enemy {
private:

	int id;
	int anchoFondo, largoFondo;
	int velocidadStandard;
	int vida;
	bool viviendo;

	bool realizandoVueltereta;

	bool llegoPuntoDeEstacionamiento;
	bool estacionando;
	Timer* t;

	Screen* screen;
	EnemyView* vistaAvion;
	ExplosionView* explosion;
	Posicion* posicion;
	GameConf* configuracion;
	int contador;
	int tiempoEntreDisparo;

	Posicion* posicionAEstacionar;

	void mostrar(int velX, int velY);
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

	int angleX;
	int angleY;

	ControllerMissilesEnemy* controlDeMisiles;
	MisilConf* misilConf;
	Resolucion* resolucion;

	Avion* avionApuntado;

public:
	Enemy(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, GameConf* conf);
	~Enemy();

	void setID( int id );
	int getID();
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();
	int getVida();

	GameConf* getConfiguracion();

	void setVelocidadStandard(int vel);

	void vivir(int velX, int velY);
	void vivirRandom();
	void vivirFlota();
	bool aunVive();

	void recibirMisil(Misil* misil);
	bool tieneHP();
	void setHP(int hp);

	Posicion* getPosicion();

	bool hit(int, int);
	void moverRandom();
	void moverFlota();
    int flota;
    int posFlota;
    bool firsttime;
    struct tm* tm;
    int secsRandom;
    chrono::time_point<chrono::system_clock> nFlota;
    void morir();
    void disparar();
    void mostrarDisparo();
    ControllerMissilesEnemy* getControllerMissiles();

    void setAvionApuntado(Avion* avion);
};

#endif /* SRC_MODEL_Enemy_H_ */
