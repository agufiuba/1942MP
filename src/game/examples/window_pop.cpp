#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

// Starts up SDL and creates window
bool initSDL(SDL_Window*&, const int SCREEN_WIDTH = 640, const int SCREEN_HEIGHT = 480);

// Fills window surface with r,g,b and updates it 
void fillWindowSurface( SDL_Window* &window, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0 );

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void closeSDL(SDL_Window*&);

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool initSDL(SDL_Window* &window, const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
  bool success = true;

  // Init SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    cout << "SDL couldn't initialize.\nSDL error: " << SDL_GetError() << endl;
    success = false;
  } else {
    // Create window
    window = SDL_CreateWindow( "Super Mario Bros", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == NULL ) {
      cout << "Window couldn't be created.\nSDL error: " << SDL_GetError() << endl;
      success = false;
    }
  }

  return success;
}

void fillWindowSurface( SDL_Window* &window, uint8_t r, uint8_t g, uint8_t b ) {
  SDL_Surface* screenSurface = SDL_GetWindowSurface( window );

  // Fill the surface with r,g,b
  SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, r, g, b ) );

  // Update the surface
  SDL_UpdateWindowSurface( window );
}

bool loadMedia(SDL_Surface* &image) {
  bool success = true;
  
  // Load splash image
  image = SDL_LoadBMP( "../images/panda.bmp" );
  if( image == NULL ) {
    cout << "Unable to load image.\nSDL error: " << SDL_GetError() << endl;
    success = false;
  }

  return success;
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
    if(!loadMedia( image )) {
     cout << "Failed to load media." << endl; 
    } else {
      // Apply the image
      SDL_BlitSurface( image, NULL, SDL_GetWindowSurface( window ), NULL );
      SDL_UpdateWindowSurface( window );
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
