#include "libs/sdl_op.h"
#include <iostream>
using namespace std;

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
}

int main() {
  // The window on which to render
  SDL_Window* window = NULL;

  // The image to load and show on screen
  SDL_Surface* image = NULL;

  // Event handler
  SDL_Event e;

  bool quit = false;

  if( !initSDL( window ) ) {
    cout << "Failed to initialize." << endl;
  } else {
    // Set BG color
    fillWindowSurface( window, 15, 36, 97 );
    if(!loadMedia( image, "../images/mario.bmp" )) {
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
