/*
 * MisilView.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "MisilView.h"

using namespace std;

MisilView::MisilView(SDL_Renderer * unRenderer, MisilConf* config) {

	rendererMisil = unRenderer;
	string filename = config->disparosSpriteID;

	if (!cargarImagenDelMisil(filename+".bmp")) {
		cout << "Vista del misil no ha sido cargada correctamente.." << endl;
		filename = filenameDefault;
		cargarImagenDelMisil(filename);
	}

}

MisilView::~MisilView() {
	vistaMisilTexture->free();
}

bool MisilView::cargarImagenDelMisil( string filename ){
	vistaMisilTexture = new Texture( rendererMisil );

	if (!vistaMisilTexture->loadFromFile(filename)) {
		printf("Failed to load Misil texture image!\n");
		return false;
	}

    return true;
}

void MisilView::mostrar( int x, int y){
    vistaMisilTexture->render( x, y );
}

int MisilView::getAncho(){
	return vistaMisilTexture->getWidth();
}

int MisilView::getLargo(){
	return vistaMisilTexture->getHeight();
}

