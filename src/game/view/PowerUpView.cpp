/*
 * PowerUpView.cpp
 *
 *  Created on: 24 de may. de 2016
 *      Author: keynaka
 */

#include "PowerUpView.h"

PowerUpView::PowerUpView(SDL_Renderer * unRenderer, string type) {
	anchoVista = 50;
	largoVista = 33;

	rendererPowerUp = unRenderer;

	string filename = "powerUp"+type+".bmp"; //TODO: se deberia sacar del XML

	if (!cargarImagenDelPowerUp(filename)) {
		cout << "Vista del powerUp no ha sido cargada correctamente.." << endl;
	}

}

PowerUpView::~PowerUpView(){
	vistaPowerUpTexture->free();
}

bool PowerUpView::cargarImagenDelPowerUp( string filename ){
	vistaPowerUpTexture = new Texture( rendererPowerUp );

	if (rendererPowerUp == NULL){
		printf("nulo");
	}

	if (!vistaPowerUpTexture->loadFromFile(filename)) {
		printf("Failed to load avion texture image!\n");
		return false;
	}

    return true;
}

int PowerUpView::getAnchoVista(){
		return anchoVista;
}

int PowerUpView::getLargoVista(){
		return largoVista;
}

void PowerUpView::mostrar( int x, int y){
	vistaPowerUpTexture->render( x, y );
}
