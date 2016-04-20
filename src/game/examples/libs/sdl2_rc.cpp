#include "sdl2_rc.h"
#include <iostream>

using namespace std;

/**
 * Starts up SDL and creates window
 *
 * @param: window	 -> points to created window on success, NULL on failure
 * @param: SCREEN_WIDTH  -> created window width (DEFAULT: 640)
 * @param: SCREEN_HEIGHT -> created window height (DEFAULT: 480)
 * @return: window creation success
 */
bool initSDL( SDL_Window*& window, SDL_Surface* &screen, const int SCREEN_WIDTH, const int SCREEN_HEIGHT ) {
  bool success = true;
  const char* WINDOW_TITLE = "1942 Stars Wars";
  window = NULL;

  // Init SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    cout << "SDL couldn't initialize.\nSDL error: " << SDL_GetError() << endl;
    success = false;
  } else {
    // Create window
    window = SDL_CreateWindow( WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == NULL ) {
      cout << "Window couldn't be created.\nSDL error: " << SDL_GetError() << endl;
      success = false;
    } else {
      int imgFlags = IMG_INIT_PNG;
      if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
	  cout<<"SDL_image could not initialize! SDL_image Error: %s\n"<< IMG_GetError()<<endl;
	  success = false;
      } else {
			screen = SDL_GetWindowSurface( window );
      }
    }
  }

  return success;
}

/**
 * Fills window surface with r, g, b and updates it
 *
 * @param: window -> pointer to created window
 * @param: r	  -> decimal color value [0..255] (DEFAULT: 0)
 * @param: g	  -> decimal color value [0..255] (DEFAULT: 0)
 * @param: b	  -> decimal color value [0..255] (DEFAULT: 0)
 * @return: void
 */
void fillWindowSurface( SDL_Window*& window,SDL_Surface* &screenSurface, uint8_t r, uint8_t g, uint8_t b ) {

  // Fill the surface with r, g, b
  SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, r, g, b ) );
  // Update the surface
  SDL_UpdateWindowSurface( window );
}

/**
 * Load BMP file
 *
 * @param: image     -> points to loaded image on success, NULL on failure
 * @param: IMAGE_URL -> relative path to image
 * @return: image load success
 */
bool loadMedia( SDL_Surface* &image , const char* IMAGE_URL) {
  bool success = true;
  image = NULL;

  // Load splash image
  image = SDL_LoadBMP( IMAGE_URL  );
  if( image == NULL ) {
    cout << "Unable to load image.\nSDL error: " << SDL_GetError() << endl;
    success = false;
  }

  return success;
}

/** Load PNG file
 *
 */
bool loadMediaPNG( SDL_Surface* &screen, SDL_Surface* &image , string IMAGE_URL) {
  bool success = true;
  //Load PNG surface
  image = loadSurface(screen, IMAGE_URL);
  if( image == NULL )
  {
  	printf( "Failed to load PNG image!\n" );
  	success = false;
  }

  return success;
}


SDL_Surface* loadSurface(SDL_Surface* &screen , string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, screen->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}
