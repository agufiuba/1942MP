#include "PlayersController.h"

using namespace std;

PlayersController::PlayersController(Vivible * unObj,SDL_Renderer* &renderer, Resolucion* resolucion){
	AvionConf* config = ((Avion*) unObj)->getConfiguracion();
	velocidadStandard = config->velocidadDesplazamiento;;
	obj = unObj;
	obj->setVelocidadStandard(velocidadStandard);
	velX = 0;
	velY = 0;
	resolucionPantalla = resolucion;

	misilConf = new MisilConf();
	misilConf->disparosSpriteID = config->disparosSpriteID;
	misilConf->velocidadDisparos = config->velocidadDisparos + config->velocidadDesplazamiento;
	controlDeMisiles = new ControllerMissiles(misilConf, renderer);
//	lastEvent = NULL;
}

PlayersController::~PlayersController(){
		delete obj;
		delete controlDeMisiles;
}

void PlayersController::press(SDL_Event *event){
//		lastEvent = event;
	cout<< "se desconecto4"<<endl;
        switch(event->key.keysym.sym){

            case SDLK_RIGHT: velX += velocidadStandard; break;
            case SDLK_LEFT: velX -= velocidadStandard; break;
            case SDLK_UP: velY += velocidadStandard; break;
            case SDLK_DOWN: velY -= velocidadStandard; break;
            case SDLK_AT: obj->desconectar(); break; // la o es una letra cualquiera. Como esta hecho con eventos, se tuvo que dejar asi
            case SDLK_KP_ENTER: obj->inicializoVueltereta();break;
            case SDLK_SPACE:
				if (!obj->haciendoVueltereta()){
					cout << "tirando misiles" << endl;
					controlDeMisiles->crearNuevoMisilEnPosicion(obj->getX()+25, obj->getY(), resolucionPantalla, misilConf);
				}
				break;
        }
}

void PlayersController::hacerVivir(){
	obj->vivir(velX, velY);
/*	velX = 0;
	velY = 0;*/
	controlDeMisiles->hacerVivir();

//  TODO: Dejo comentado otra solucion alternativa para que los aviones no se muevan sin control
//	if (lastEvent != NULL) {
//		switch(lastEvent->key.keysym.sym){
//			case SDLK_RIGHT: velX -= velocidadStandard; obj->vivir(velX, velY); break;
//			case SDLK_LEFT: velX += velocidadStandard; obj->vivir(velX, velY); break;
//			case SDLK_UP: velY -= velocidadStandard; obj->vivir(velX, velY); break;
//			case SDLK_DOWN: velY += velocidadStandard; obj->vivir(velX, velY); break;
//		}
//		lastEvent = NULL;
//
//	}
}
