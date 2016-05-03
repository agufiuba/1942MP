/*
 * MisilView.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_VIEW_MISILVIEW_H_
#define SRC_GAME_VIEW_MISILVIEW_H_


#include "Texture.h"
#include <string>
#include <iostream>

using namespace std;

class MisilView {
	private:

		Texture *vistaMisilTexture = NULL;
		SDL_Renderer *rendererMisil;

//		int anchoVista;
//		int largoVista;

	public:
		MisilView(SDL_Renderer* renderer);
		virtual ~MisilView();
		bool cargarImagenDelMisil( string filename );
//		int getAnchoVista();
//		int getLargoVista();
		void mostrar(int x, int y);
};

#endif /* SRC_GAME_VIEW_MISILVIEW_H_ */
