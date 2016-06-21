#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include "../data/player_data.h"
#include "../data/player_status.h"
#include "../data/player_score.h"
#include "../data/planes_actives.h"
#include "../data/active_players.h"
#include "../data/stage_data.h"
#include "../data/game_data.h"
#include "../data/enemy_data.h"
#include "../data/enemy_status.h"
#include "../data/powerup_status.h"
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
#include "../../xml/conf/AvionConf.h"
#include "../../xml/conf/ElementoConf.h"
#include "../../xml/conf/PowerUpConf.h"
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
    bool sendData( PlayerScore* data, string id = "SP" );
    bool sendData( PlanesActives* data );
    bool sendData( EnemyData* data );
    bool sendData( EnemyStatus* data );
    bool sendData( PlayerStatus* data );
    bool sendData( PlayerData* data, string s );
    bool sendData( Evento* e );
    bool sendData( AvionConf* e );
    bool sendData( ElementoConf* e );
    bool sendData( PowerUpConf* e );
    bool sendData( EnemigoConf* e );
    bool sendData( EscenarioConf* e );
    bool sendData( SpriteConf* e );
    bool sendData( StageData* data, string id );
    bool sendData( ActivePlayers* data );
    bool sendData( GameData* data );
    bool sendData( PowerUpStatus* data );
    bool sendEndDataConf( int cantidad );
    bool sendDataDisconnect( PlayerData* data );
    bool sendGetConfig();

    int receiveData( char id[2], int size);
    int receiveData( PlayerData* data );
    int receiveData( PlayerScore* data );
    int receiveData( StageData* data );
    int receiveData( PlayerStatus* data );
    int receiveData( PlanesActives* data );
    int receiveData( EnemyData* data );
    int receiveData( EnemyStatus* data );
    int receiveData( AvionConf* data );
    int receiveData( ElementoConf* data );
    int receiveData( PowerUpConf* data );
    int receiveData( EnemigoConf* data );
    int receiveData( EscenarioConf* data );
    int receiveData( SpriteConf* data );
    int receiveData( Evento* e );
    int receiveData( ActivePlayers* data );
    int receiveData( char data[1] );
    int receiveData( GameData* data );
};
#endif
