#include "PlayersController.h"

using namespace std;

PlayersController::PlayersController(Vivible * unObj,SDL_Renderer* &renderer){
	velocidadStandard = 7;
	obj = unObj;
	obj->setVelocidadStandard(velocidadStandard);
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
            case SDLK_d: velX += velocidadStandard; break;
            case SDLK_a: velX -= velocidadStandard; break;

            case SDLK_w: velY += velocidadStandard; break;
            case SDLK_s: velY -= velocidadStandard; break;

        //    case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;
        }
    }
    else if( event->type == SDL_KEYUP && event->key.repeat == 0){
        switch( event->key.keysym.sym ){
            case SDLK_d: velX -= velocidadStandard; break;
            case SDLK_a: velX += velocidadStandard; break;

            case SDLK_w: velY -= velocidadStandard; break;
            case SDLK_s: velY += velocidadStandard; break;

            /*case SDLK_SPACE: controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(),obj->getY()); break;*/
        }
    }
}

void PlayersController::hacerVivir(){
	obj->vivir(velX, velY);

}
