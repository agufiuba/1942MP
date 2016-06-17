/*
 * ServerAvionEnemigoRandom.cpp
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoRandom.h"

ServerAvionEnemigoRandom::ServerAvionEnemigoRandom(Posicion* posicionInicial):ServerAvionEnemigo(posicionInicial) {

}

ServerAvionEnemigoRandom::~ServerAvionEnemigoRandom(){
	delete t;
	delete posicion;
}

void ServerAvionEnemigoRandom::vivir() {
	enum Direction { U, D, R, L };
	Direction d;
	int x, y;
	time_t temp = time(0);
	localtime(&temp);

	if ((tm->tm_sec - secsRandom) < 1 && !firsttime) {
		x = angleX;
		y = angleY;
	} else {
		srand(time(NULL));
		d = static_cast<Direction>( rand() % 5 );

		switch( d ) {
		  case U:
		    cout << "UP" << endl;
		    break;
		  case D:
		    cout << "DOWN" << endl;
		    break;
		  case R:
		    cout << "RIGHT" << endl;
		    break;
		  case L:
		    cout << "LEFT" << endl;
		    break;
		  default:
		    break;
		}
		
		firsttime = false;
		secsRandom = tm->tm_sec;
	}
}

