#ifndef SRC_GAME_VIEW_ENEMYVIEW_H_
#define SRC_GAME_VIEW_ENEMYVIEW_H_

#include "SDL2/SDL.h"

#include "Texture.h"
#include <string>
#include <iostream>

using namespace std;

class EnemyView {

public:
	EnemyView(SDL_Renderer * unRenderer, string tipo);
	~EnemyView();
	bool cargarImagenDelAvion( string filename );
	int getAnchoVista();
	int getLargoVista();
	void mostrarRotado(int x, int y, int velX, int velY);
	void cargarClips();

private:
	string dir = "src/game/images/";
	int anchoVista;
	int largoVista;
	string tipo;
	Texture *vistaAvionTexture = NULL;
	SDL_Renderer *rendererAvion;
	SDL_Rect clipsDerecha[ 2 ];
	SDL_Rect clipsIzquierda[ 2 ];

};

#endif /* SRC_GAME_VIEW_ENEMYVIEW_H_ */
