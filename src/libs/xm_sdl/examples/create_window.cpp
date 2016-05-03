#include "../XM_SDL.h"

int main() {

  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;
  const char* WINDOW_TITLE = "Full Metal Panic! Last Raid";

  XM_SDL* sdlHandler = new XM_SDL( SDL_INIT_VIDEO );

  if( sdlHandler->createWindow( WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT ) ) {
    sdlHandler->setWindowBG( 254, 125, 90 );
    sdlHandler->setWindowBG( "images/background.bmp" );

    while( true ) {
      if( sdlHandler->nextEvent().type == SDL_QUIT ) {
	break;		
      }
    }
  }

  delete sdlHandler;

  return 0;
}
