#include "Controller.h"

using namespace std;

Controller::Controller(Vivible * unObj,SDL_Renderer* renderer, Resolucion* resolucion, Client* client){
	this->cliente = client;
	AvionConf* conf = ((Avion*) unObj)->getConfiguracion();
	velocidadStandard = conf->velocidadDesplazamiento;
	obj = unObj;
	obj->setVelocidadStandard(velocidadStandard);
	velX = 0;
	velY = 0;
	resolucionPantalla = resolucion;

	envioExplosion = false;

	misilConf = new MisilConf();
	strcpy(misilConf->disparosSpriteID,conf->disparosSpriteID);
	misilConf->velocidadDisparos = conf->velocidadDisparos + conf->velocidadDesplazamiento;
 	controlDeMisiles = new ControllerMissiles(misilConf, renderer);
}

Controller::~Controller(){
		delete obj;
		delete controlDeMisiles;
		delete misilConf;
}

void Controller::press(SDL_Event *event){

	if (!obj->tieneHP()) return;

	Evento* e;
	CompanionEvent* ce = new CompanionEvent();

    if( event->type == SDL_KEYDOWN && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX += velocidadStandard; cliente->sendData(ce->right(obj->getId())); break;
            case SDLK_LEFT: velX -= velocidadStandard; cliente->sendData(ce->left(obj->getId())); break;

            case SDLK_UP: velY += velocidadStandard; cliente->sendData(ce->up(obj->getId())); break;
            case SDLK_DOWN: velY -= velocidadStandard; cliente->sendData(ce->down(obj->getId()));break;

            case SDLK_KP_ENTER:
            	if (!obj->haciendoVueltereta()) {
            		obj->inicializoVueltereta();
            		cliente->sendData(ce->roll(obj->getId()));
            	}
            	break;


            case SDLK_p: obj->inicializoEstacionar(); cliente->sendData(ce->estaciono(obj->getId())); break;

            case SDLK_i: obj->desconectar(); break;

            case SDLK_r: cliente->sendData(ce->restart(obj->getId()));break;

            case SDLK_ESCAPE: this->cliente->quitGame(); break;

            case SDLK_s:
            	if (cliente->getGameData()->practiceMode){
            		cout<<"SALIMOS DEL MODO PRACTICA"<<endl;
				 	cliente->getGameData()->practiceMode = false;
            		cliente->sendData(ce->outPractice(obj->getId()));
            	}
				break;

            case SDLK_SPACE:
				if (!obj->haciendoVueltereta() && !obj->estaEstacionando()) {
					controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX() + 12,obj->getY(), resolucionPantalla, misilConf);
					cliente->sendData(ce->shot(obj->getId()));
				}
				break;

        }
    }
    else if( event->type == SDL_KEYUP && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX -= velocidadStandard; cliente->sendData(ce->left(obj->getId())); break;
            case SDLK_LEFT: velX += velocidadStandard; cliente->sendData(ce->right(obj->getId())); break;

            case SDLK_UP: velY -= velocidadStandard; cliente->sendData(ce->down(obj->getId())); break;
            case SDLK_DOWN: velY += velocidadStandard; cliente->sendData(ce->up(obj->getId())); break;

            /*case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;*/
        }
    }
}

void Controller::hacerVivir(){
//	Evento* e;
//	CompanionEvent* ce = new CompanionEvent();
//
//	if (!obj->tieneHP() && !envioExplosion) {
//		cliente->sendData(ce->explosion(obj->getId()));
//		envioExplosion = true;
//		delete ce;
//	}

	obj->vivir(velX, velY);
	controlDeMisiles->hacerVivir();

	this->actualizarPosicionAvionEnServer();
}

Vivible* Controller::getVivible(){
	return obj;
}

void Controller::actualizarPosicionAvionEnServer(){

	if (!obj->tieneHP()) return;

	PlayerData* p = new PlayerData();

	strcpy(p->name, obj->getId().c_str());
	p->x = obj->getX();
	p->y = obj->getY();

	while (!this->cliente->sendDataPosicion(p));
	usleep(100);

	delete p;
}

void Controller::setAmetralladora(){
	controlDeMisiles->setAmetralladora();
}

void Controller::setBalaNormal(){
	controlDeMisiles->setBalaNormal();
}

