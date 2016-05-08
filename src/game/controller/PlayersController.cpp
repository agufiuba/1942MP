#include "PlayersController.h"

using namespace std;

PlayersController::PlayersController(Vivible * unObj,SDL_Renderer* &renderer, Resolucion* resolucion){
	velocidadStandard = 7;
	obj = unObj;
	obj->setVelocidadStandard(velocidadStandard);
	velX = 0;
	velY = 0;
	resolucionPantalla = resolucion;
	controlDeMisiles = new ControllerMissiles(renderer);
}

PlayersController::~PlayersController(){
		delete obj;
		delete controlDeMisiles;
}

void PlayersController::press(SDL_Event *event){

        switch(event->key.keysym.sym){

            case SDLK_RIGHT: velX += velocidadStandard; break;
            case SDLK_LEFT: velX -= velocidadStandard; break;
            case SDLK_UP: velY += velocidadStandard; break;
            case SDLK_DOWN: velY -= velocidadStandard; break;
            case SDLK_KP_ENTER: obj->inicializoVueltereta();break;
            case SDLK_SPACE:
				if (!obj->haciendoVueltereta()){
					cout << "tirando misiles" << endl;
					controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX(), obj->getY(), resolucionPantalla);
				}
				break;
        }
}

void PlayersController::hacerVivir(){
	obj->vivir(velX, velY);
}
