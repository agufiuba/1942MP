#include "XM_SDL.h"
#include <iostream>
#include <cstdlib>

using namespace std;

XM_SDL::XM_SDL( uint32_t flags ) {
/*
  enum InitFlags {
    SDL_INIT_TIMER,	     // timer subsystem.
    SDL_INIT_AUDIO,	     // audio subsystem.
    SDL_INIT_VIDEO,	     // video subsystem. Automatically initializes the SDL_INIT_EVENTS subsystem. 
    SDL_INIT_JOYSTICK,	     // joystick subsystem.
    SDL_INIT_HAPTIC,	     // haptic (force feedback) subsystem.
    SDL_INIT_GAMECONTROLLER, // controller subsystem. Automatically initializes the SDL_INIT_JOYSTICK subsystem.
    SDL_INIT_EVENTS,	     // events subsystem.
    SDL_INIT_EVERYTHING,     // all of the above subsystems.
    SDL_INIT_NOPARACHUTE     // compatibility. This flag is ignored.
  };
*/
  // Initialize SDL
  if( SDL_Init( flags ) != 0 ) {
    cout << endl << "Unable to initialize SDL: " << SDL_GetError() << endl;
    exit(1);
  }

  this->window = NULL;
  this->renderer = NULL;
  this->windowBG = NULL;
}

XM_SDL::~XM_SDL() {
  // Deallocate surfaces
  for(vector<SDL_Texture*>::iterator it = this->loadedTextures.begin(); 
      it != this->loadedTextures.end();
      ++it) {
    SDL_DestroyTexture( *it );
  }

  // Destroy window
  if( this->window != NULL )
    SDL_DestroyWindow( this->window );

  // Destroy renderer
  if( this->renderer != NULL )
    SDL_DestroyRenderer( this->renderer );

  // Quit SDL subsystems
  SDL_Quit();
}

bool XM_SDL::createWindow( const char* TITLE, const int WIDTH, const int HEIGHT ) {
  // Create window
  this->window = SDL_CreateWindow( TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
				   WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if( this->window == NULL ) {
    cout << endl << "Unable to create window: " << SDL_GetError() << endl;
    return false;
  }

  // Create renderer for window
  this->renderer = SDL_CreateRenderer( this->window, -1, SDL_RENDERER_ACCELERATED );
  
  if( this->renderer == NULL ) {
    cout << endl << "Renderer could not be created: " << SDL_GetError() << endl;
    return false;
  }

  // Initialize renderer color
  SDL_SetRenderDrawColor( this->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );

  return true;
}

SDL_Surface* XM_SDL::getWindowSurface() {
  // Get window surface
  return SDL_GetWindowSurface( this->window );
}

void XM_SDL::setWindowBG( const uint8_t RED, const uint8_t GREEN, const uint8_t BLUE ) {
  SDL_SetRenderDrawColor( this->renderer, RED, GREEN, BLUE, SDL_ALPHA_OPAQUE );
  // Clear the screen with the render color
  SDL_RenderClear( this->renderer );

  this->updateWindow();
}

void XM_SDL::updateWindow() {
  // Update window with previous rendering
  SDL_RenderPresent( this->renderer ); 
}

SDL_Texture* XM_SDL::createTexture( SDL_Surface*& surface ) {
  SDL_Texture* texture = NULL;

  texture = SDL_CreateTextureFromSurface( this->renderer, surface );
  // Get rid of old loaded surface
  SDL_FreeSurface( surface );

  return texture;
}

SDL_Texture* XM_SDL::loadImage( const char* IMAGE_PATH ) {
  SDL_Surface* image = NULL;
  image = SDL_LoadBMP( IMAGE_PATH );
  SDL_Texture* texture = NULL;
  
  if( image == NULL ) {
    cout << "Unable to load image: " << SDL_GetError() << endl;
  } else {
    // Try create texture from surface
    texture = this->createTexture( image );
    if( texture == NULL ) {
      cout << "Unable to create texture for image ( " << IMAGE_PATH << " ) : "
	   << SDL_GetError() << endl;
    } else {
      this->loadedTextures.push_back( texture );
    }
  }

  return texture;
}

void XM_SDL::setWindowBG( const char* IMAGE_PATH ) {
  SDL_Texture* image = this->loadImage( IMAGE_PATH );
  
  if( image != NULL ) {
    SDL_RenderCopy( this->renderer, image, NULL, NULL );
    this->updateWindow();
  }
}

SDL_Event XM_SDL::nextEvent() {
  SDL_Event e;
  // Get next event in queue
  SDL_PollEvent( &e );

  return e;
}
