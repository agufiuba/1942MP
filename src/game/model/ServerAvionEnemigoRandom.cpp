/*
 * ServerAvionEnemigoRandom.cpp
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoRandom.h"

ServerAvionEnemigoRandom::ServerAvionEnemigoRandom( int id, Posicion* posicionInicial
):ServerAvionEnemigo(id, posicionInicial) {
  this->randomSeed = id;
  this->dAnt = 4;
}

ServerAvionEnemigoRandom::~ServerAvionEnemigoRandom(){
	delete t;
	delete posicion;
}

void ServerAvionEnemigoRandom::refreshSeed() {
  this->randomSeed += ( this->randomSeed << 10 );
  this->randomSeed ^= ( this->randomSeed >> 6 ); 
  this->randomSeed += ( this->randomSeed << 3 ); 
}

EnemyData* ServerAvionEnemigoRandom::vivir() {
	this->refreshSeed();
	enum Direction { U, D, R, L };
	Direction d;
	EnemyData* ed = new EnemyData;
	ed->id = this->id;
      	srand(time(NULL));
	d = static_cast<Direction>( ( rand() + this->randomSeed ) % 4 );
	if ( this->dAnt != 4 ) {
	  while ( d == static_cast<Direction>( this->dAnt ) ) { 
	    this->refreshSeed();
	    d = static_cast<Direction>( ( rand() + this->randomSeed ) % 4 );
	  }
	}
		switch( d ) {
		  case U:
		    cout << "UP" << endl;
		    ed->direction = 'U';	  
		    this->dAnt = 0;
		    break;
		  case D:
		    cout << "DOWN" << endl;
		    ed->direction = 'D';	  
		    this->dAnt = 1;
		    break;
		  case R:
		    cout << "RIGHT" << endl;
		    ed->direction = 'R';	  
		    this->dAnt = 2;
		    break;
		  case L:
		    cout << "LEFT" << endl;
		    ed->direction = 'L';	  
		    this->dAnt = 3;
		    break;
		  default:
		    cout << "DEFAULT" << endl;
		    if ( this->dAnt == 1 ) {
		      ed->direction = 'U';	  
		      this->dAnt = 0;
		    } else {
		      ed->direction = 'D';	  
		      this->dAnt = 1;
		    }
		}
		
	return ed;
}
