#include "Controller.h"

using namespace std;

Controller::Controller(Vivible * unObj){
	obj = unObj;
	velX = 0;
	velY = 0;
}

Controller::~Controller(){
		delete obj;
}

void Controller::press(SDL_Event *event){

    if( event->type == SDL_KEYDOWN ){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX += obj->getAncho() / distanciaRecorrida; break;
            case SDLK_LEFT: velX -= obj->getAncho() / distanciaRecorrida; break;

            case SDLK_UP: velY -= obj->getLargo() / distanciaRecorrida; break;
            case SDLK_DOWN: velY += obj->getLargo() / distanciaRecorrida; break;
        }
    }
    else if( event->type == SDL_KEYUP ){
        switch( event->key.keysym.sym ){
            case SDLK_RIGHT: velX -= obj->getAncho() / distanciaRecorrida; break;
            case SDLK_LEFT: velX += obj->getAncho() / distanciaRecorrida; break;

            case SDLK_UP: velY += obj->getLargo() / distanciaRecorrida; break;
            case SDLK_DOWN: velY -= obj->getLargo() / distanciaRecorrida; break;
        }

    }
}

void Controller::hacerVivir(){
	obj->vivir(velX, velY);
}
