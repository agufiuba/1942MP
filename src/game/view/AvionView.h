#ifndef SRC_VIEW_AVIONVIEW_H_
#define SRC_VIEW_AVIONVIEW_H_

#include "SDL2/SDL.h"

#include "Texture.h"
#include <string>
#include <iostream>

using namespace std;

class AvionView {

private:
	//The surfaces
	Texture *vistaAvionTexture = NULL;
	SDL_Renderer *rendererAvion;
	SDL_Rect clipsDerecha[ 2 ];
	SDL_Rect clipsIzquierda[ 2 ];
	SDL_Rect clipsVueltereta[ 6 ];

	int anchoVista;
	int largoVista;

public:
	bool cargarImagenDelAvion( string filename );
	AvionView(SDL_Renderer* renderer, string color);
	~AvionView();
	int getAnchoVista();
	int getLargoVista();
	void mostrar(int x, int y, int velX);
	void mostrarVueltereta(int x, int y, int frame);
	void cargarClips();
};

#endif /* SRC_VIEW_AVIONVIEW_H_ */
