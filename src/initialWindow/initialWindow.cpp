#include "../libs/xm_sdl/XM_SDL.h"
#include "../game/view/Texture.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {

	const int WINDOW_WIDTH = 600; //TODO: aca hay que cambiar a lo del escenario
	const int WINDOW_HEIGHT = 600;
	const char* WINDOW_TITLE = "1942MP Arcade";

	bool quit = false;

	XM_SDL* sdlHandler = new XM_SDL( SDL_INIT_EVERYTHING);

	// Create window
	if (sdlHandler->createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT)) {
		SDL_Event event;
		SDL_Renderer* renderer = sdlHandler->getRenderer();

		Texture * logoPrincipal = new Texture();

		if (!logoPrincipal->loadFromFile("windowImages/Arcade_-_1942_-_General_Sprites.bmp", renderer)) {
			printf("Failed to load logoPrincipal texture image!\n");
			return false;
		}

		while (!quit) {

			// Get events
			while (sdlHandler->nextEvent(&event)) {

				if (event.type == SDL_QUIT)
					quit = true;
			}

			// Set window background
			sdlHandler->setWindowBG(0, 0, 0);

			sdlHandler->updateWindow();

		}

	}

	delete sdlHandler;

return 0;
}


