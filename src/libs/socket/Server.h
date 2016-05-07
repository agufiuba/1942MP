#ifndef SERVER_H
#define SERVER_H
#include "../logger/Logger.h"
#include "../../xml/conf/ServerConf.h"
#include "../../game/events/Events.cpp"
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
    void sendData( int clientFD, Evento* data, int dataLength );

  public:
    Server( const char* configFileName );
    ~Server();
    void initialize();
    void shutdown();
};
#endif
