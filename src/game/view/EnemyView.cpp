/*
 * EnemyView.cpp
 *
 *  Created on: 11 de jun. de 2016
 *      Author: ramon
 */

#include "EnemyView.h"
#include <math.h>

using namespace std;

EnemyView::EnemyView(SDL_Renderer * unRenderer, string tipo){
	this->tipo = tipo;
	rendererAvion = unRenderer;

	if (!cargarImagenDelAvion(tipo)) {
		cout << "Vista del avion no ha sido cargada correctamente.." << endl;
	}

	anchoVista = 50;
	largoVista = 50;
	cargarClips();

}

EnemyView::~EnemyView(){
	vistaAvionTexture->free();
}


int EnemyView::getAnchoVista(){
		return anchoVista;
}

int EnemyView::getLargoVista(){
		return largoVista;
}

bool EnemyView::cargarImagenDelAvion(string filename){
	vistaAvionTexture = new Texture( rendererAvion );

	if (!vistaAvionTexture->loadFromFile(filename)) {
		printf("Failed to load Misil texture image!\n");
		return false;
	}

    return true;
}


void EnemyView::cargarClips(){

			clipsIzquierda[ 0 ].x = 0;
			clipsIzquierda[ 0 ].y = 0;
			clipsIzquierda[ 0 ].w = anchoVista;
			clipsIzquierda[ 0 ].h = largoVista;
}

void EnemyView::mostrarRotado( int x, int y, int velX, int velY){
  bool izquierda = true;
  int frame = 0;
  SDL_Rect* currentClip = &clipsIzquierda[0];
  if(velX == 0 && velY == 0) {
  	velY = -1;
  	velX = 0;
  }
  double mod = pow(velX * velX + velY * velY, 0.5);
  double angle = asin(velY / mod);
  angle = angle * 180 / M_PI;
  if (velX < 0) angle  = 180 - angle;
  vistaAvionTexture->renderWithAngle( x, y, currentClip, 90 - angle);
}
