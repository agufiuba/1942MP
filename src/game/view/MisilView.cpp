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
	string imageId = config->disparosSpriteID;

	string filename = dir + imageId + ".bmp";

	if (!cargarImagenDelMisil(filename)) {
		cout << "Vista del misil no ha sido cargada correctamente.." << endl;
		filename = dir + filenameDefault;
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
