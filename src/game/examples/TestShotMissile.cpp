#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include "../../game/examples/libs/sdl2_rc.h"

using namespace std;

const int SCREEN_WIDTH = 400; //x
const int SCREEN_HEIGHT = 600; //y
int widthImage;
int heigthImage;
int posInicialNaveX = 125;
int posInicialNaveY = 450;

// The window on which to render
SDL_Window* window = NULL;

// The image to load and show on screen
SDL_Surface* image = NULL;

SDL_Rect dest;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
Texture* naveTexture = NULL;
Texture* misilTexture = NULL;
Texture* backgroundTexture = NULL;

const Uint8 *keys;

struct nave {
		int x, y;
};

struct misil {
		int x, y;
};

nave* minave = NULL;
misil* mimisil = NULL;

mutex themutex;



class Renderer{
private:
	bool active;

public:
	Renderer();
	void renderizer();
	bool isActive();
	void setActive();
};

Renderer::Renderer(){
	active = false;
	printf("Inicia Renderer...");
}

void Renderer::renderizer(){
	//Clear screen
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

			SDL_RenderClear(gRenderer);

			backgroundTexture->render(0, 0, gRenderer);

			naveTexture->render(minave->x, minave->y, gRenderer);

			if(misilTexture != NULL){

				misilTexture->render(mimisil->x, mimisil->y, gRenderer);

			}
			SDL_RenderPresent(gRenderer);

}

bool Renderer::isActive(){
	return active;
}
void Renderer::setActive(){
	active = !active;
}
Renderer* renderer = new Renderer();


int shotMissile(void* data ) {

	misilTexture = new Texture();
	mimisil = new misil();

	if (!loadMediaTexture(misilTexture, "../images/naveEspacial.bmp", gRenderer)) {
		cout << "Failed to load media." << endl;
	} else {

		mimisil->x = minave->x + (naveTexture->getWidth()/2) - (misilTexture->getWidth()/2);
		mimisil->y = minave->y-200;

		cout<<"misil en pos "<<to_string(mimisil->y)<<endl;

		while ( mimisil->y > 0 ) {

			mimisil->y = mimisil->y - 20;
			usleep(400000);
			cout<<"misil en pos "<<to_string(mimisil->y)<<endl;
//			renderer->renderizer();
		}
		misilTexture->free();
	}

	return 0;
}

void close() {
	//Free loaded images
	naveTexture->free();
	backgroundTexture->free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
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

	minave = new nave();

	if (!initSDL(window, gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT)) {
		cout << "Failed to initialize." << endl;
	} else {

		// Set BG color
		if (!loadMediaTexture(naveTexture, "../images/naveEspacial.bmp", gRenderer)) {
			cout << "Failed to load media." << endl;
		} else {

			if(!loadMediaTexture(backgroundTexture,"../images/space_background.bmp", gRenderer)){
				cout << "Failed to load media." << endl;
			} else {
				widthImage = naveTexture->getWidth();
				heigthImage = naveTexture->getHeight();
				minave->x = posInicialNaveX;
				minave->y = posInicialNaveY;

				renderer->renderizer();

			}
		}
	}

	while (!quit) {
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			//User presses a key
			else if (e.type == SDL_KEYDOWN) {
				///keys = SDL_GetKeyboardState(NULL);
				if (e.key.keysym.sym == SDLK_UP && minave->y > 0) {
					minave->y = minave->y - (1);
				}
				if (e.key.keysym.sym == SDLK_DOWN
				    && minave->y < (SCREEN_HEIGHT - heigthImage)) {
					minave->y = minave->y + (1);
				}
				if (e.key.keysym.sym == SDLK_LEFT && minave->x > 0) {
					minave->x = minave->x - (1);
				}
				if (e.key.keysym.sym == SDLK_RIGHT
				    && minave->x < (SCREEN_WIDTH - widthImage)) {
					minave->x = minave->x + (1);
				}
				if (e.key.keysym.sym == SDLK_SPACE) {
//					shotMissile(\);
					int data = 1;
					 SDL_Thread* threadID = SDL_CreateThread( shotMissile, "LazyThread", (void*)data );
					 SDL_DetachThread(threadID);
//					 SDL_WaitThread( threadID, NULL );
				}
				if (e.key.keysym.sym == SDLK_r) {
					minave->x = posInicialNaveX;
					minave->y = posInicialNaveY;
				}
			}
		}

		renderer->renderizer();
	}

	close();

	return 0;
}
