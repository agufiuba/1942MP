#ifndef CLIENT_H
#define CLIENT_H
#include "../data/player_data.h"
#include "../data/player_status.h"
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
    PlanesActives* planes;
	vector<ElementoConf*> elementos;
	vector<SpriteConf*> sprites;
    vector<PlayerData*> allPlayers;

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
    bool sendDataDisconnect( PlayerData* data);
    PlanesActives* getPlanesActives();
    bool isPlayerOk();
    bool sendData( Evento* e );
    bool allPlayersReady();
    bool sendPlayerDisconnection();
    void setHandler(HandlerPlayersControllers* handlerPlayersControllers);
    vector<PlayerData*> getPlayers();
    GameConf* getConfig();
};
#endif
