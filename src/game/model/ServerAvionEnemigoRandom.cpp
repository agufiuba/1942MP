/*
 * ServerAvionEnemigoRandom.cpp
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoRandom.h"

ServerAvionEnemigoRandom::ServerAvionEnemigoRandom( int id, Posicion* posicionInicial
):ServerAvionEnemigo(id, posicionInicial) {

}

ServerAvionEnemigoRandom::~ServerAvionEnemigoRandom(){
	delete t;
	delete posicion;
}

EnemyData* ServerAvionEnemigoRandom::vivir() {
	enum Direction { U, D, R, L };
	Direction d;
	EnemyData* ed = new EnemyData;
	ed->id = this->id;
	time_t temp = time(0);
	localtime(&temp);

	if ((tm->tm_sec - secsRandom) < 1 && !firsttime) {
	  ed->direction = 'N';	  
	} else {
		srand(time(NULL));
		d = static_cast<Direction>( rand() % 5 );

		switch( d ) {
		  case U:
		    cout << "UP" << endl;
		    ed->direction = 'U';	  
		    break;
		  case D:
		    cout << "DOWN" << endl;
		    ed->direction = 'D';	  
		    break;
		  case R:
		    cout << "RIGHT" << endl;
		    ed->direction = 'R';	  
		    break;
		  case L:
		    cout << "LEFT" << endl;
		    ed->direction = 'L';	  
		    break;
		  default:
		    break;
		}
		
		firsttime = false;
		secsRandom = tm->tm_sec;
	}

	return ed;
}
