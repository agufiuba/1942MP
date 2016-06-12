#ifndef CLIENT_H
#define CLIENT_H
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
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Client {
  private:
    HandlerPlayersControllers* pc;
    HandlerPowerUp* hPowerUp;
    string ip;
    string puerto;
    Player* player;
    int teamScore;
    int rivalTeamScore;
    int socketFD;
    bool connected;
    bool received;
    bool playerOk;
    Logger* logger;
    int stageOffset;
    PlanesActives* planes;
	vector<ElementoConf*> elementos;
	vector<PowerUpConf*> powerUps;
	vector<SpriteConf*> sprites;
    vector<PlayerData*> allPlayers;
    vector<PlayerScore*> playersScoreData;
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

    bool destroyEne;

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
    void setNotDestroyEnemys();
    void setDestroyEnemys();
    bool destroyEnemys();
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
    int getTeamScore();
    int getRivalTeamScore();
    void setTeamScore( int score );
    void setRivalTeamScore( int score );
    void quitGame();
};
#endif
