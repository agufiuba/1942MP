#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "../data/player_data.h"
#include "../data/planes_actives.h"
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
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
    bool sendData( PlayerData* data, string s );
    bool sendData( GameConf* );
    bool receiveData( char id[2], int size , int & b);
    bool receiveData( PlayerData* data , int & b);
    bool receiveData( PlanesActives* data , int & b);
    bool receiveData( GameConf* data , int & b);
	
	bool sendData( Evento* e );
    bool receiveData( Evento* e , int & b);
};
#endif
