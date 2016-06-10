#ifndef SERVER_H
#define SERVER_H
#include "../logger/Logger.h"
#include "../../xml/conf/GameConf.h"
#include "../../game/events/Events.cpp"
#include "../../libs/data/player_data.h"
#include "../../libs/data/player_status.h"
#include "../../libs/data/player_score.h"
#include "../../libs/data/active_players.h"
#include "../../libs/data/planes_actives.h"
#include "../../libs/data/stage_data.h"
#include "../../libs/data/game_data.h"
#include "../../game/model/Player.h"
#include <queue>
#include <map>
#include <string>
#include <iostream>
using namespace std;

class Server {
  private:
    int socketFD;
    int clientCount;
    int maxClientCount;
    int readyPlayers;
    bool listening;
    bool connected;
    bool allowConnections;
    bool processing;
    bool running;
    GameData* gameData;
    StageData* stageData;
    queue<map<int, Evento*>*>* eventQueue;
    map<int, Player*> players;

    //vector<int> players2;
    int posicionInicialX;
    int posicionInicialY;

    Logger* logger;
    GameConf* config;
    static const int BACKLOG = 5;
    static const int MAX_UNREACHABLE_TIME = 5;
    void listenForConnections( int clientFD, struct sockaddr_storage clientAddress );
    void checkAliveSend( int clientFD );
    void receiveClientData( int clientFD, struct sockaddr_storage clientAddress );
    void closeClient( int clientFD );
    void* getInAddr( struct sockaddr* sa );
    void closeConnection();
    void processQueue();
//    bool processMsg( string type, string value );
    //void sendData( int clientFD, Evento* data, int dataLength );
    void sendData( int clientFD, Evento* data);
    bool receiveData( char id[2], int clientFD, int size );
    bool receiveData( PlayerData* data, int clientFD );
    void addPlayer( PlayerData* data, int clientFD );
    void updatePlayerStatus( PlayerStatus* data, int clientFD );
    void sendPlanesActives(int cfd);
    void sendGameData(int cfd);
    void sendGameDataAll();
    void sendConf(int);
    void addScoreToPlayer( PlayerScore* data );
    void sendScoreTable( int clientFD );
    void createPlayers();
    void queryCurrentStageOffset();
    void sendCurrentStageOffset( int clientFD );
    void removeAllPlayers();
    void sendActivePlayers( int clientFD );
    int getActivePlayersCount();
    void sendStageReadySignal();
    void sendPlayersReady();
    void setTeamPlayer(int team, int cfd);
    int getPlayerTeamScore( Player* player );
    int getPlayerRivalTeamScore( Player* player );

  public:
    Server( const char* configFileName );
    ~Server();
    void createGameData();
    void initialize();
    void shutdown();
    void avisarDesconexionDeAvion(int cfd);
};
#endif
