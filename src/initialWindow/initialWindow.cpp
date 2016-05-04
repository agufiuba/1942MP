#include "../libs/xm_sdl/XM_SDL.h"
#include "../game/view/Texture.h"
#include "../game/controller/Timer.h"

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
    SDL_Rect* exactImage = new SDL_Rect();
    exactImage->x = 0;
    exactImage->y = 0;
    exactImage->w = 250;
    exactImage->h = 100;

    if (!logoPrincipal->loadFromFile("windowImages/1942logoPrincipal.bmp", renderer)) {
      printf("Failed to load logoPrincipal texture image!\n");
      return false;
    }

    // Set window background
    sdlHandler->setWindowBG(0, 0, 0);

    logoPrincipal->render(175,75,renderer,exactImage);

    sdlHandler->updateWindow();

    while (!quit) {
      // Get events
      while (sdlHandler->nextEvent(&event)) {
	if (event.type == SDL_QUIT)
	  quit = true;
      }
    }
  }

  delete sdlHandler;

  return 0;
}


