#ifndef CLIENT_H
#define CLIENT_H
#include "../data/player_data.h"
#include "../data/player_status.h"
#include "../data/player_score.h"
#include "../data/active_players.h"
#include "../data/planes_actives.h"
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
#include "../../xml/conf/ClientConf.h"
#include "../../game/events/Events.cpp"
#include "../../game/controller/HandlerPlayersControllers.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Client {
  private:
    HandlerPlayersControllers* pc;
    string ip;
    string puerto;
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
    void setHandler(HandlerPlayersControllers* handlerPlayersControllers);
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
    void requestClientsPlaying();
    int getClientsPlaying();
    void resetClientsPlaying();
    void sendStageClearReady();
    bool stageClearReady;
};
#endif
