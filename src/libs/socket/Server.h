#ifndef SERVER_H
#define SERVER_H
#include "../logger/Logger.h"
#include "../../xml/conf/ServerConf.h"
#include "../../game/events/Events.cpp"
#include "../../libs/data/player_data.h"
#include "../../game/model/Player.h"
#include <queue>
#include <map>
#include <string>
using namespace std;

class Server {
  private:
    int socketFD;
    int clientCount;
    bool listening;
    bool connected;
    bool allowConnections;
    bool processing;
    queue<map<int, Evento*>*>* eventQueue;
    map<int, Player*> players;

    vector<int> players2;

    Logger* logger;
//    ServerConf* config;
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

  public:
    Server( const char* configFileName );
    ~Server();
    void initialize();
    void shutdown();
};
#endif
