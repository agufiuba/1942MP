#ifndef CLIENT_H
#define CLIENT_H
#include "../data/player_data.h"
#include "../data/planes_actives.h"
#include "../logger/Logger.h"
#include "../../xml/conf/ClientConf.h"
#include "../../game/events/Events.cpp"

class Client {
  private:
    string ip;
    string puerto;
    int socketFD;
    bool connected;
    bool received;
    bool playerOk;
    Logger* logger;
    PlanesActives* planes;

    static const int MAX_UNREACHABLE_TIME = 5;
    //    ClientConf* config;
    void checkAliveSend();
    void receiving( const int MAX_DATA_SIZE, const char* IP );
    void closeConnection();
    bool sendData( Evento* e );

  public:
    Client( const char* configFileName );
    Client( string ip, string puerto );
    ~Client();
    bool connectToServer();
    void disconnectFromServer();
    void shutdownConnection();
//  vector<Evento*> getEventos();
    void sendCycle();
    bool sendData( PlayerData* data );
    PlanesActives* getPlanesActives();
    bool isPlayerOk();
};
#endif
