#ifndef CLIENT_H
#define CLIENT_H
#include "../data/enemy_data.h"
#include "../data/enemy_status.h"
#include "../data/powerup_data.h"
#include "../data/player_data.h"
#include "../data/player_status.h"
#include "../data/player_score.h"
#include "../data/active_players.h"
#include "../data/planes_actives.h"
#include "../data/game_data.h"
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
#include "../../xml/conf/ClientConf.h"
#include "../../game/events/Events.cpp"
#include "../../game/model/Player.h"
#include "../../game/model/Enemy.h"
#include "../../game/controller/HandlerPlayersControllers.h"
#include "../../game/controller/HandlerPowerUp.h"
#include "../../game/controller/HandlerEnemigos.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Client {
  private:
    HandlerPlayersControllers* pc;
    HandlerPowerUp* hPowerUp;
    HandlerEnemigos* hEnemigos;
    string ip;
    string puerto;
    Player* player;
    int coopTeamScore;
    int alphaTeamScore;
    int betaTeamScore;
    int socketFD;
    bool connected;
    bool received;
    bool playerOk;
    bool winner;
    bool loser;
    Logger* logger;
    int stageOffset;
    PlanesActives* planes;
	vector<ElementoConf*> elementos;
	vector<EnemigoConf*> enemigos;
	vector<SpriteConf*> sprites;
    vector<PlayerData*> allPlayers;
    vector<PlayerScore*> playersScoreData;
    vector<EnemyStatus*> enemys;
    vector<PowerUpData*> powerUps;
    int clientsPlaying;

    static const int MAX_UNREACHABLE_TIME = 5;
    void checkAliveSend();
    void receiving( const int MAX_DATA_SIZE, const char* IP );
    void closeConnection();
    GameConf* config;
    bool configComplete;
    bool ready;
    GameData* gameData;
    bool playerResume;
    void sendEnemyMovements( EnemyStatus* data );

  public:
    Client( const char* configFileName );
    Client( string ip, string puerto );
    Client (string ip, string puerto, HandlerPlayersControllers* handlerPlayersControllers);
    ~Client();
    bool connectToServer();
    void disconnectFromServer();
    void shutdownConnection();
    //  vector<Evento*> getEventos();
    void sendCycle();
    bool sendData( PlayerData* data );
    bool sendStageData();
    bool sendScore( PlayerScore* data );
    bool sendDataPosicion( PlayerData* data );
    PlanesActives* getPlanesActives();
    bool isPlayerOk();
    bool sendData( Evento* e );
    bool allPlayersReady();
    bool sendPlayerDisconnection();
    bool sendPlayerDeath();
    void setHandler(HandlerPlayersControllers* handlerPlayersControllers);
    void setPowerUpHandler(HandlerPowerUp* hPowerUp);
    void setEnemyHandler(HandlerEnemigos* hEnemigos);
    vector<PlayerData*> getPlayers();
    vector<PlayerScore*> getPlayersScoreData();
    GameConf* getConfig();
    bool isConfigComplete();
    void setConfigComplete(bool);
    bool reset;
    bool reinicia;
    bool sendGetConfig();
    void resetConfig();
    void setStageOffset( int offset );
    int getStageOffset();
    bool isConnected();
    void resetScores();
    void requestScoreTable();
    int getClientsPlaying();
    void resetClientsPlaying();
    void sendStageClearReady();
    bool stageClearReady;
    void addScoreToPlayer( int score );
    void requestPlayerScore();
    void requestScoreReset();
    void setPlayer( Player* player );
    GameData* getGameData();
    void sendGameData();
    void sendMode(string mode);
    void sendStatusReady();
    bool isPlayerResume();
    int getCoopTeamScore();
    int getAlphaTeamScore();
    int getBetaTeamScore();
    void setCoopTeamScore( int score );
    void setAlphaTeamScore( int score );
    void setBetaTeamScore( int score );
    void quitGame();
    bool wins();
    bool losses();
    void setCoopMode( bool mode );
    void setTeamMode( bool mode );
    void sendEnemyDeath( int id = -1 );
    void sendRecoverEnemysAlive();
    vector<EnemyStatus*> getEnemys();
    vector<PowerUpData*> getPowerUps();
    void resetEnemys();
    void resetPowerUps();
    void removeEnemy( vector<EnemyStatus*>::iterator it );
    void requestEnemyMovements( int id );
    void sendEnemyHit( int enemyID, string playerID );
    bool crearPowerUpGrande;
    Posicion* powerUpGrande;
    void sendPowerUpHit( int powerUpID );
};
#endif
