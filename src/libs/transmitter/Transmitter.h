#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "../data/player_data.h"
#include "../data/planes_actives.h"
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
#include <string>
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
    bool sendData( GameConf* );
    bool receiveData( char id[2], int size );
    bool receiveData( PlayerData* data );
    bool receiveData( PlanesActives* data );
    bool receiveData( GameConf* data );
};
#endif
