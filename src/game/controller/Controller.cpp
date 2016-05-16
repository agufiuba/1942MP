#include "Controller.h"

using namespace std;

Controller::Controller(Vivible * unObj,SDL_Renderer* &renderer, Resolucion* resolucion, Client* client){
	this->cliente = client;
	AvionConf* conf = ((Avion*) unObj)->getConfiguracion();
	velocidadStandard = conf->velocidadDesplazamiento;
	obj = unObj;
	obj->setVelocidadStandard(velocidadStandard);
	velX = 0;
	velY = 0;
	resolucionPantalla = resolucion;

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
  Evento* e;
  CompanionEvent* ce = new CompanionEvent();

    if( event->type == SDL_KEYDOWN && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX += velocidadStandard; cliente->sendData(ce->right(obj->getId())); break;
            case SDLK_LEFT: velX -= velocidadStandard; cliente->sendData(ce->left(obj->getId())); break;

            case SDLK_UP: velY += velocidadStandard; cliente->sendData(ce->up(obj->getId())); break;
            case SDLK_DOWN: velY -= velocidadStandard; cliente->sendData(ce->down(obj->getId()));break;

            case SDLK_KP_ENTER: obj->inicializoVueltereta(); cliente->sendData(ce->roll(obj->getId())); break;

            case SDLK_i: obj->desconectar(); break;

            case SDLK_r: cout<<"Se Reinicia la partida"<<endl;cliente->sendData(ce->restart(obj->getId()));break;

			case SDLK_SPACE:
				if (!obj->haciendoVueltereta())
					controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX()+25, obj->getY(), resolucionPantalla, misilConf);
					cliente->sendData(ce->shot(obj->getId()));
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
	obj->vivir(velX, velY);
	controlDeMisiles->hacerVivir();

	this->actualizarPosicionAvionEnServer();
}

Vivible* Controller::getVivible(){
	return obj;
}

void Controller::actualizarPosicionAvionEnServer(){
	PlayerData* p = new PlayerData();

	strcpy(p->name, obj->getId().c_str());
	p->x = obj->getX();
	p->y = obj->getY();

	while (!this->cliente->sendDataPosicion(p));
	usleep(100);
}
