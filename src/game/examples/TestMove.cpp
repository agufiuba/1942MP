#include "libs/sdl2_rc.h"
#include <iostream>
using namespace std;

const int BPP = 24;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;


// The window on which to render
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

// The image to load and show on screen
SDL_Surface* image = NULL;

SDL_Rect dest;

const Uint8 *keys;

struct nave{
    int x,y;
} minave;

void applyMedia(SDL_Window* &window, SDL_Surface* &image, nave* minave) {
	dest.x = minave->x;
	dest.y = minave->y;
	dest.w = image -> w;
	dest.h = image -> h;

  // Apply the image
  SDL_BlitSurface( image, NULL, SDL_GetWindowSurface( window ), &dest );
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

  bool quit = false;

  //Event handler
  SDL_Event e;

  nave* minave = new nave();

  if( !initSDL( window , screenSurface ,SCREEN_WIDTH, SCREEN_HEIGHT) ) {
    cout << "Failed to initialize." << endl;
  } else {
    // Set BG color
    fillWindowSurface( window, screenSurface, 0, 0, 0);
    if(!loadMediaPNG( screenSurface, image, "../images/nave.png")) {
     cout << "Failed to load media." << endl;
    } else {
      minave->x = 10;
      minave->y = 10;

      applyMedia( window, image, minave );
    }
  }

  while( !quit ) {
    // Handle events on queue
    while( SDL_PollEvent(&e) != 0 ) {
      // User requests quit
      if( e.type == SDL_QUIT ) {
    	  quit = true;
      }
      //User presses a key
      else if( e.type == SDL_KEYDOWN )
	  {
    	  ///keys = SDL_GetKeyboardState(NULL);
    	  if(e.key.keysym.sym == SDLK_UP && minave->y > 0) {
    		  minave->y = minave->y - (15);
    	  }
    	  if(e.key.keysym.sym == SDLK_DOWN && minave->y < SCREEN_HEIGHT) {
    		  minave->y = minave->y + (15);
    	  }
    	  if(e.key.keysym.sym == SDLK_LEFT && minave->x > 0) {
    		  minave->x = minave->x - (15);
    	  }
    	  if(e.key.keysym.sym == SDLK_RIGHT && minave->x < SCREEN_WIDTH) {
    		  minave->x = minave->x + (15);
    	  }
      }
    }
    applyMedia( window, image, minave );
  }


  closeSDL(window, image);

  return 0;
}
