#include <iostream>

#include "../../game/examples/libs/sdl2_rc.h"

using namespace std;

const int SCREEN_WIDTH = 400;//x
const int SCREEN_HEIGHT = 600;//y
int widthImage;
int heigthImage;

// The window on which to render
SDL_Window* window = NULL;

// The image to load and show on screen
SDL_Surface* image = NULL;

SDL_Rect dest;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
Texture* naveTexture = NULL;
Texture* backgroundTexture = NULL;


const Uint8 *keys;

struct nave{
    int x,y;
} minave;

void applyMedia(SDL_Window* &window, SDL_Surface* &image, nave* minave) {
	dest.x = minave->x;
	dest.y = minave->y;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_HEIGHT;

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
}



void close(){
	//Free loaded images
	naveTexture->free();

	backgroundTexture->free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( window );
	window = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

int main() {

  bool quit = false;

  //Event handler
  SDL_Event e;

  naveTexture = new Texture();
  backgroundTexture = new Texture();

  nave* minave = new nave();

  if( !initSDL( window ,gRenderer,SCREEN_WIDTH, SCREEN_HEIGHT) ) {
    cout << "Failed to initialize." << endl;
  } else {
    // Set BG color
    //fillWindowSurface( window, 80, 100, 0);
    //if(!loadMedia( image, "../images/nave_espacial.bmp")) {

    if(!loadMediaTexture( naveTexture, "../images/naveEspacial.bmp",gRenderer)) {
    	cout << "Failed to load media." << endl;
    } else {

    	if (!loadMediaTexture(backgroundTexture, "../images/space_background.bmp", gRenderer)){
    		cout << "Failed to load media." << endl;
    	} else {

				widthImage = naveTexture->getWidth();
				heigthImage = naveTexture->getHeight();
				//widthImage = image->w;
				//heigthImage = image->h;
				minave->x = 125;
				minave->y = 450;

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				SDL_RenderClear( gRenderer );

				//Render background texture to screen
				backgroundTexture->render( 0, 0 ,gRenderer);

				//Render Foo' to the screen
				naveTexture->render( minave->x, minave->y, gRenderer);

				//Update screen

				SDL_RenderPresent( gRenderer );
				//applyMedia( window, image, minave );
    	}
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
    		  minave->y = minave->y - (40);
    	  }
    	  if(e.key.keysym.sym == SDLK_DOWN && minave->y < (SCREEN_HEIGHT-heigthImage)) {
    		  minave->y = minave->y + (40);
    	  }
    	  if(e.key.keysym.sym == SDLK_LEFT && minave->x > 0) {
    		  minave->x = minave->x - (40);
    	  }
    	  if(e.key.keysym.sym == SDLK_RIGHT && minave->x < (SCREEN_WIDTH-widthImage)) {
    		  minave->x = minave->x + (40);
    	  }
      }
    }

		//Clear screen
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );

		//Render background texture to screen
		backgroundTexture->render( 0, 0 ,gRenderer);

		//Render Foo' to the screen
		naveTexture->render( minave->x, minave->y ,gRenderer);

		//Update screen
		SDL_RenderPresent( gRenderer );
    //applyMedia( window, image, minave );
  }


//  closeSDL(window, image);

  close();

  return 0;
}
