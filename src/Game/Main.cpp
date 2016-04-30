#include "Control/Timer.h"
#include "Model/Avion.h"
#include "Control/Controller.h"

#include <iostream>

const int SCREEN_WIDTH = 600; //TODO: aca hay que cambiar a lo del escenario
const int SCREEN_HEIGHT = 600;

SDL_Renderer* renderer;
SDL_Window * screen;
SDL_Event event;

const int FRAMES_PER_SECOND = 10;

using namespace std;

bool init(){

    //Inicializa SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

  	//Crea la Pantalla
    SDL_Window * screen = SDL_CreateWindow("1942MP Arcade", SDL_WINDOWPOS_UNDEFINED,
  	SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if( screen == NULL ) {
  		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  		return false;
    }

  	//Crear Renderer
  	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
  	if (renderer == NULL) {
  		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  		return false;
  	}

  	//Initialize renderer color
  	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}

void close(){
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( screen );
	screen = NULL;
	renderer = NULL;

	SDL_Quit();
}


int main(int argc, char **argv) {

  if( init() == false ){
  		cout << "No cargo bien el screen"<< endl;
      return 1;
  }

	bool quit = false;

  Timer fps;

  Vivible* unAvion = new Avion(renderer);
  Controller* control = new Controller(unAvion);

  while(!quit){
  		fps.correr();

      while( SDL_PollEvent( &event ) ){

          control->press( &event);

          if( event.type == SDL_QUIT ){ quit = true; }
      }


    	//Clear screen
    	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);

      //Clear screem
			SDL_RenderClear(renderer);

      control->hacerVivir();

			//Update screen
			SDL_RenderPresent(renderer);

      if( fps.tiempoActual() < 1000 / FRAMES_PER_SECOND ){
          SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.tiempoActual() );
      }
  }

  close(); //TODO: aca adentro esta haciendo delete del escenario tmb. Posiblemente a cambiar luego.

	delete control;

	return 0;
}
