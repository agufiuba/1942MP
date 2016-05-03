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
  this->windowBG = NULL;
}

XM_SDL::~XM_SDL() {
  // Deallocate surfaces
  for(vector<SDL_Surface*>::iterator it = this->loadedMedia.begin(); 
      it != this->loadedMedia.end();
      ++it) {
    SDL_FreeSurface( *it );
  }

  // Destroy window
  if( this->window != NULL )
    SDL_DestroyWindow( this->window );

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

  return true;
}

SDL_Surface* XM_SDL::getWindowSurface() {
  // Get window surface
  return SDL_GetWindowSurface( this->window );
}

void XM_SDL::updateWindowSurface() {
  // Update the surface
  SDL_UpdateWindowSurface( this->window );
}

void XM_SDL::setWindowBG( const uint8_t RED, const uint8_t GREEN, const uint8_t BLUE ) {
  this->windowBG = this->getWindowSurface();
  
  // Fill BG surface with RGB
  SDL_FillRect( this->windowBG, NULL, SDL_MapRGB( this->windowBG->format, RED, GREEN, BLUE ) );

  this->updateWindowSurface();
}

bool XM_SDL::optimizeSurface( SDL_Surface*& surface ) {
  SDL_Surface* oldSurface = surface;
  surface = NULL;

  surface = SDL_ConvertSurface( oldSurface, this->getWindowSurface()->format, 0 );
  if( surface == NULL ) {
    surface = oldSurface;   
    return false;
  }
  
  // Get rid of old loaded surface
  SDL_FreeSurface( oldSurface );

  return true;
}

SDL_Surface* XM_SDL::loadImage( const char* IMAGE_PATH ) {
  SDL_Surface* image = NULL;
  image = SDL_LoadBMP( IMAGE_PATH );

  if( image == NULL ) {
    cout << "Unable to load image: " << SDL_GetError() << endl;
  } else {
    // Try to optimize image
    if( !( this->optimizeSurface( image ) ) )
      cout << "Couldn't optimize image: " << IMAGE_PATH << endl;
    this->loadedMedia.push_back( image );
  }

  return image;
}

void XM_SDL::setWindowBG( const char* IMAGE_PATH ) {
  SDL_Surface* image = this->loadImage( IMAGE_PATH );
  
  if( image != NULL ) {
    SDL_BlitSurface( image, NULL, this->getWindowSurface(), NULL ); 
    this->updateWindowSurface();
  }
}

SDL_Event XM_SDL::nextEvent() {
  SDL_Event e;
  // Get next event in queue
  SDL_PollEvent( &e );

  return e;
}
