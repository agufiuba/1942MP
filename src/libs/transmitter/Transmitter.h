#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "../data/player_data.h"
#include "../data/player_status.h"
#include "../data/planes_actives.h"
#include "../data/stage_data.h"
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

  public:
    Transmitter( int peerFD, Logger* logger );
    ~Transmitter();
    bool sendDataID( string id );
    bool sendData( PlayerData* data );
    bool sendData( PlanesActives* data );
    bool sendData( PlayerStatus* data );
    bool sendData( PlayerData* data, string s );
    bool sendData( Evento* e );
    bool sendData( AvionConf* e );
    bool sendData( ElementoConf* e );
    bool sendData( EscenarioConf* e );
    bool sendData( SpriteConf* e );
    bool sendData( StageData* data, string id );
    bool sendEndDataConf( int cantidad );
    bool sendDataDisconnect( PlayerData* data );
    bool sendGetConfig();

    int receiveData( char id[2], int size);
    int receiveData( PlayerData* data );
    int receiveData( StageData* data );
    int receiveData( PlayerStatus* data );
    int receiveData( PlanesActives* data );
    int receiveData( AvionConf* data );
    int receiveData( ElementoConf* data );
    int receiveData( EscenarioConf* data );
    int receiveData( SpriteConf* data );
    int receiveData( Evento* e );
    int receiveData( char data[1] );
};
#endif
