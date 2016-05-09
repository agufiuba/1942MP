#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "../data/player_data.h"
#include "../logger/Logger.h"
#include "../../game/events/CompanionEvent.h"
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
    bool receiveData( char id[2], int size );
    bool receiveData( PlayerData* data );

    bool sendData( Evento* e );
    bool receiveData( Evento* e );
};
#endif
