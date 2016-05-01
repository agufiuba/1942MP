/*
 * MisilView.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "MisilView.h"

using namespace std;

MisilView::MisilView(SDL_Renderer * unRenderer) {
	//TODO pasarlo a sprites
//	anchoVista = 120;
//	largoVista = 120;


	rendererMisil = unRenderer;

	if (!cargarImagenDelMisil("images/ball.bmp")) {
		cout << "Vista del misil no ha sido cargada correctamente.." << endl;
		//TODO: faltaria que tire una excepcion
	}

}

MisilView::~MisilView() {
	vistaMisilTexture->free();
}


bool MisilView::cargarImagenDelMisil( string filename ){
	vistaMisilTexture = new Texture();

	if (rendererMisil == NULL){
		printf("nulo");
	}

	if (!vistaMisilTexture->loadFromFile(filename, rendererMisil)) {
		printf("Failed to load Misil texture image!\n");
		return false;
	}

    return true;
}

//int MisilView::getAnchoVista(){
//		return anchoVista;
//}
//
//int MisilView::getLargoVista(){
//		return largoVista;
//}


void MisilView::mostrar( int x, int y){
		//cout << "x " << x << "       y "<< y <<endl;

    vistaMisilTexture->render( x, y, rendererMisil, NULL );
}
