#include "PlayersController.h"

using namespace std;

PlayersController::PlayersController(Vivible * unObj,SDL_Renderer* &renderer){
	obj = unObj;
	velX = 0;
	velY = 0;
  //controlDeMisiles = new ControllerMissiles(renderer);
}

PlayersController::~PlayersController(){
		delete obj;
	//	delete controlDeMisiles;
}

void PlayersController::press(SDL_Event *event){

    if( event->type == SDL_KEYDOWN && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_d: velX += obj->getAncho() / distanciaDeDesplazamiento; break;
            case SDLK_a: velX -= obj->getAncho() / distanciaDeDesplazamiento; break;

            case SDLK_w: velY -= obj->getLargo() / distanciaDeDesplazamiento; break;
            case SDLK_s: velY += obj->getLargo() / distanciaDeDesplazamiento; break;

        //    case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;
        }
    }
    else if( event->type == SDL_KEYUP && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_d: velX -= obj->getAncho() / distanciaDeDesplazamiento; break;
            case SDLK_a: velX += obj->getAncho() / distanciaDeDesplazamiento; break;

            case SDLK_w: velY += obj->getLargo() / distanciaDeDesplazamiento; break;
            case SDLK_s: velY -= obj->getLargo() / distanciaDeDesplazamiento; break;

            /*case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;*/
        }
    }
}

void PlayersController::hacerVivir(){
	obj->vivir(velX, velY);
	//controlDeMisiles->hacerVivir();
}
