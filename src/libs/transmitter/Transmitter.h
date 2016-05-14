#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "../data/player_data.h"
#include "../data/player_status.h"
#include "../data/planes_actives.h"
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
#include "../../xml/conf/AvionConf.h"
#include "../../xml/conf/ElementoConf.h"
#include "../../xml/conf/EscenarioConf.h"
#include "../../xml/conf/SpriteConf.h"
#include <string>
#include "../../game/events/CompanionEvent.h"

using namespace std;

class Transmitter {
  private:
    int peerFD;
    Logger* logger;
    bool sendDataID( string id );

  public:
    Transmitter( int peerFD, Logger* logger );
    ~Transmitter();
    bool sendData( PlayerData* data );
    bool sendData( PlanesActives* data );
    bool sendData( PlayerStatus* data );
    bool sendData( PlayerData* data, string s );
    bool receiveData( char id[2], int size , int & b);
    bool receiveData( PlayerData* data , int & b);
    bool receiveData( PlayerStatus* data , int & b);
    bool receiveData( PlanesActives* data , int & b);
	
	bool sendData( Evento* e );
    bool receiveData( Evento* e , int & b);
	bool sendData( AvionConf* e );
	bool sendData( ElementoConf* e );
	bool sendData( EscenarioConf* e );
	bool sendData( SpriteConf* e );
	bool receiveData( AvionConf* data );
	bool receiveData( ElementoConf* data );
	bool receiveData( EscenarioConf* data );
	bool receiveData( SpriteConf* data );

	bool sendEndDataConf(int cantidad);
	bool receiveData( char data[1] );
	bool sendDataDisconnect( PlayerData* data );

};
#endif
