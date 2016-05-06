#include "Controller.h"

using namespace std;

Controller::Controller(Vivible * unObj,SDL_Renderer* &renderer){
	velocidadStandard = 7;
	obj = unObj;
	obj->setVelocidadStandard(velocidadStandard);
	velX = 0;
	velY = 0;
  controlDeMisiles = new ControllerMissiles(renderer);
}

Controller::~Controller(){
		delete obj;
		delete controlDeMisiles;
}

void Controller::press(SDL_Event *event){

    if( event->type == SDL_KEYDOWN && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX += velocidadStandard; break;
            case SDLK_LEFT: velX -= velocidadStandard; break;

            case SDLK_UP: velY += velocidadStandard; break;
            case SDLK_DOWN: velY -= velocidadStandard; break;

            case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;
        }
    }
    else if( event->type == SDL_KEYUP && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX -= velocidadStandard; break;
            case SDLK_LEFT: velX += velocidadStandard; break;

            case SDLK_UP: velY -= velocidadStandard; break;
            case SDLK_DOWN: velY += velocidadStandard; break;

            /*case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;*/
        }
    }
}

void Controller::hacerVivir(){
	obj->vivir(velX, velY);
	controlDeMisiles->hacerVivir();
}
