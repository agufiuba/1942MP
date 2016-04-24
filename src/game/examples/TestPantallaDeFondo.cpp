#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "libs/Texture.h"
using namespace std;

SDL_Window* window = NULL;
Texture* fondoDePantalla = NULL;
SDL_Rect posicion;
SDL_Event evento;
SDL_Renderer* gRenderer = NULL;

bool isFullScreen = true;
bool quit = false;

int SCREEN_WIDTH = 1366;
int SCREEN_HEIGHT = 768;
const int FRAMES_PER_SECOND = 60;

const char* DIR_FONDO_PANTALLA = "src/game/images/fondoPantalla-1800.bmp";
const char* WINDOW_TITLE = "1942 MultiPlayer";


/**
 * Esta funcion permite setear un tamaño de pantalla fijo, cuando no es full screen
 * Incluir libreria: #include <X11/Xlib.h>
 */
//void setearTamanoPantalla() {
//	int bordes = 100;
//	Display* d = XOpenDisplay(NULL);
//	Screen* s = DefaultScreenOfDisplay(d);
//	SCREEN_WIDTH = s->width - bordes;
//	SCREEN_HEIGHT = s->height - bordes;
//}

void printErrorSDL(string error) {
	cout << "No se puede iniciarlizar "<< error << endl;
	cout << "SDL Error: " << SDL_GetError() << endl;
}

bool initSDL() {

	window = NULL;
	fondoDePantalla = new Texture();

	// Inicializar SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printErrorSDL("SDL");
		return false;
	}

	// Inicializar window
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL) {
		printErrorSDL("window");
		return false;
	}

	// Inicializar window FullScreen
	if (isFullScreen && SDL_SetWindowFullscreen(window, SDL_TRUE) < 0) {
		// TODO: En el futuro sacar el isFullScreen
		printErrorSDL("Full Screen");
		return false;
	}

	// Inicializar Renderer
	gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printErrorSDL("Renderer");
		return false;
	}

	// Load fondo de pantalla
	if (!fondoDePantalla->loadFromFile(DIR_FONDO_PANTALLA, gRenderer)) {
		printErrorSDL("Fondo de Pantalla");
		return false;
	}

	return true;
}

void actualizarFondoDePantalla(int x, int y) {
	SDL_RenderClear(gRenderer);
	posicion.x = x;
	posicion.y = y;
	fondoDePantalla->render(x, y, gRenderer);
	SDL_RenderPresent(gRenderer);
}

int moverFondoDePantalla(int y) {
	if (y > -1100) {
		cout << "baja y: " << y << endl;
		y-=3;
	}
	return y;
}

void aplicarFPS(Uint32 start) {
	Uint32 dif = (SDL_GetTicks() - start);
	int time = 1000 / FRAMES_PER_SECOND;
	if(time > dif) {
		SDL_Delay( time - dif);
	}
}

void closeSDL() {
	fondoDePantalla->free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(window);
	gRenderer = NULL;
	window = NULL;
	SDL_Quit();
}

int main() {
	cout << "Empezando test..." << endl;

//	if (!fullScreen) {
//		setearTamanoPantalla();
//	}
	if (!initSDL()) {
		return 0;
	}

	int x = 0;
	int y = 0;
	actualizarFondoDePantalla(x, y);

	cout << "SCREEN_WIDTH: " << SCREEN_WIDTH << endl;
	cout << "SCREEN_HEIGHT: " << SCREEN_HEIGHT << endl;
	cout << "actual: " << posicion.y << endl;
	Uint32 start;
	while (!quit) {
		start = SDL_GetTicks();
		while( SDL_PollEvent(&evento) != 0 ) {
			if(evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q) {
				quit = true;
				break;
			}

		}

		y = moverFondoDePantalla(y);
		actualizarFondoDePantalla(x, y);
		aplicarFPS(start);

	}

	closeSDL();
	cout << "Finalizando test..." << endl;
	return 0;
}

