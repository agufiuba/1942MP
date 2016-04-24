#include "../../../game/examples/libs/sdl2_rc.h"

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
bool initSDL(SDL_Window*& window, SDL_Renderer*& gRenderer,
    const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
	bool success = true;
	const char* WINDOW_TITLE = "1942 Multiplayer";
	window = NULL;

	// Init SDL
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		cout << "SDL couldn't initialize.\nSDL error: " << SDL_GetError() << endl;
		return !success;
	}

	// Create window
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << "Window couldn't be created.\nSDL error: " << SDL_GetError()
		    << endl;
		return !success;
	}

	//Create renderer for window
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return !success;
	}

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

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
void fillWindowSurface(SDL_Window*& window, uint8_t r, uint8_t g, uint8_t b) {
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

	// Fill the surface with r, g, b
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, r, g, b));
	// Update the surface
	SDL_UpdateWindowSurface(window);
}

/**
 * Load BMP file
 *
 * @param: image     -> points to loaded image on success, NULL on failure
 * @param: IMAGE_URL -> relative path to image
 * @return: image load success
 */
bool loadMedia(SDL_Surface* &image, const char* IMAGE_URL) {
	bool success = true;
	image = NULL;

	// Load splash image
	image = SDL_LoadBMP(IMAGE_URL);
	if (image == NULL) {
		cout << "Unable to load image.\nSDL error: " << SDL_GetError() << endl;
		success = false;
	}

	return success;
}

bool loadMediaTexture(Texture* &imageTexture, const char* IMAGE_URL, SDL_Renderer* &renderer) {
	bool success = true;

//Load texture
	if (!imageTexture->loadFromFile(IMAGE_URL, renderer)) {
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	return success;
}
