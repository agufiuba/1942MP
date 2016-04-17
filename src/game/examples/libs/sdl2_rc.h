#ifndef SDL_OP_H
#define SDL_OP_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;
/**
 * Starts up SDL and creates window
 *
 * @param: window	 -> points to created window on success, NULL on failure
 * @param: SCREEN_WIDTH  -> created window width
 * @param: SCREEN_HEIGHT -> created window height
 * @return: window creation success
 */
bool initSDL( SDL_Window*& window, SDL_Surface*& screenSurface, const int SCREEN_WIDTH, const int SCREEN_HEIGHT );

/**
 * Fills window surface with r, g, b and updates it
 *
 * @param: window -> pointer to created window
 * @param: r	  -> decimal color value (0..255)
 * @param: g	  -> decimal color value (0..255)
 * @param: b	  -> decimal color value (0..255)
 * @return: void
 */
void fillWindowSurface( SDL_Window*& window, SDL_Surface*& screenSurface, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0 );

/**
 * Load BMP file
 *
 * @param: image     -> points to loaded image on success, NULL on failure
 * @param: IMAGE_URL -> relative path to image
 * @return: image load success
 */
bool loadMedia( SDL_Surface* &image, const char* IMAGE_URL );


bool loadMediaPNG( SDL_Surface* &gScreenSurface, SDL_Surface* &image, string IMAGE_URL );

//Loads individual image
SDL_Surface* loadSurface(SDL_Surface* &gScreenSurface, std::string path );

#endif
