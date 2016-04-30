#ifndef SRC_VIEW_AVIONVIEW_H_
#define SRC_VIEW_AVIONVIEW_H_

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <string>
#include <iostream>

using namespace std;

class AvionView {

private:
	//The surfaces
	SDL_Surface *vistaAvion = NULL;
	SDL_Surface *screen;

	SDL_Rect clipsDerecha[ 2 ];
	SDL_Rect clipsIzquierda[ 2 ];

	int anchoVista;
	int largoVista;

public:
	SDL_Surface* cargarImagenDelAvion( string filename );
	AvionView(SDL_Surface* unEscenario);
	~AvionView();
	int getAnchoVista();
	int getLargoVista();
	void mostrar(int x, int y);
	void cargarClips();
	void cargarmeEnEscenario( int x, int y, SDL_Surface* obj, SDL_Surface* escenario, SDL_Rect* clip = NULL );
};

#endif /* SRC_VIEW_AVIONVIEW_H_ */
