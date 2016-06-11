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
#include "../../xml/conf/MisilConf.h"

using namespace std;

class MisilView {
	private:

		Texture *vistaMisilTexture = NULL;
		SDL_Renderer *rendererMisil;
		string dir = "src/game/images/";
		string filenameDefault = "desconocido_chico.bmp";

	public:
		MisilView(SDL_Renderer* renderer, MisilConf* config);
		virtual ~MisilView();
		bool cargarImagenDelMisil( string filename );
		void mostrar(int x, int y);
		int getAncho();
		int getLargo();
};

#endif /* SRC_GAME_VIEW_MISILVIEW_H_ */
