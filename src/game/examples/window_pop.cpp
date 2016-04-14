#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main() {
  // The window on which to render
  SDL_Window* window = NULL;
  // The surface contained by the window
  SDL_Surface* screenSurface = NULL;

  // Init SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    cout << "SDL couldn't initialize.\nSDL error: " << SDL_GetError() << endl;
  } else {
    // Create window
    window = SDL_CreateWindow( "Super Mario Bros", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == NULL ) {
      cout << "Window couldn't be created.\nSDL error: " << SDL_GetError() << endl;
    } else {
      // Get window surface
      screenSurface = SDL_GetWindowSurface( window );
      
      // Fill the surface with white
      SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 15, 36, 97 ) );

      // Update the surface
      SDL_UpdateWindowSurface( window );

      // Wait two seconds
      SDL_Delay( 2000 );
    }
  }

  // Destroy window
  SDL_DestroyWindow( window );
  
  // Quit SDL subsystems
  SDL_Quit();
  
  return 0;
}
