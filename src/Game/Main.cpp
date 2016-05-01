#include "Control/Timer.h"
#include "Model/Avion.h"
#include "Control/Controller.h"
#include "../libs/xm_sdl/XM_SDL.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {

  const int WINDOW_WIDTH = 600; //TODO: aca hay que cambiar a lo del escenario
  const int WINDOW_HEIGHT = 600;
  const char* WINDOW_TITLE = "1942MP Arcade";
  const int FRAMES_PER_SECOND = 10;
  bool quit = false;

  XM_SDL* sdlHandler = new XM_SDL( SDL_INIT_EVERYTHING );
  
  // Create window
  if( sdlHandler->createWindow( WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT ) ) {
    SDL_Event event;
    SDL_Renderer* renderer = sdlHandler->getRenderer();
    Timer fps;

    Vivible* unAvion = new Avion( renderer );
    Controller* control = new Controller( unAvion );
    ControllerMissiles* controlDeMisiles = new ControllerMissiles(unAvion,renderer);

    while( !quit ) {
      fps.correr();
      
      // Get events
      while( sdlHandler->nextEvent( &event ) ) {
	control->press( &event );
        controlDeMisiles->press(&event);
	if( event.type == SDL_QUIT ) quit = true;
      }

      // Set window background
      sdlHandler->setWindowBG( 0, 127, 255 );
      control->hacerVivir();
      controlDeMisiles->hacerVivir();
      sdlHandler->updateWindow();

      if( fps.tiempoActual() < 1000 / FRAMES_PER_SECOND ){
	SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.tiempoActual() );
      }
      
    } 

    delete sdlHandler;
    delete control;
  } 

  return 0;  
}
