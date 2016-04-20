#include "libs/sdl2_rc.h"
#include <iostream>
using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;

void applyMedia(SDL_Window* &window, SDL_Surface* &image) {



  // Apply the image
  SDL_BlitSurface( image, NULL, SDL_GetWindowSurface( window ), NULL );
  // Update the surface
  SDL_UpdateWindowSurface( window );
}

void closeSDL(SDL_Window* &window, SDL_Surface* &image) {
  // Deallocate surface
  SDL_FreeSurface( image );
  // Destroy window
  SDL_DestroyWindow( window );
  // Quit SDL subsystems
  SDL_Quit();
  IMG_Quit();
}

int main() {
  // The window on which to render
  SDL_Window* window = NULL;

  //The surface contained by the window
  SDL_Surface* screenSurface = NULL;

  // The image to load and show on screen
  SDL_Surface* image = NULL;

  // Event handler
  SDL_Event e;

  bool quit = false;

  if( !initSDL( window , screenSurface ,SCREEN_WIDTH, SCREEN_HEIGHT) ) {
    cout << "Failed to initialize." << endl;
  } else {
    // Set BG color
    fillWindowSurface( window, screenSurface, 0, 0, 0);
    if(!loadMediaPNG( screenSurface, image, "../images/nave_espacial.png" )) {
     cout << "Failed to load media." << endl;
    } else {
      applyMedia( window, image );
    }
  }

  while( !quit ) {
    // Handle events on queue
    while( SDL_PollEvent(&e) != 0 ) {
      // User requests quit
      if( e.type == SDL_QUIT ) {
	quit = true;
      }
    }
  }

  closeSDL( window, image );

  return 0;
}
