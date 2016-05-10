#ifndef CLIENT_H
#define CLIENT_H
#include "../data/player_data.h"
#include "../logger/Logger.h"
#include "../../xml/conf/ClientConf.h"
#include "../../game/events/Events.cpp"
#include "../../game/controller/HandlerPlayersControllers.h"

class Client {
  private:
		HandlerPlayersControllers* pc;
    string ip;
    string puerto;
    int socketFD;
    bool connected;
    bool received;
    Logger* logger;
    static const int MAX_UNREACHABLE_TIME = 5;
    //    ClientConf* config;
    void checkAliveSend();
    void receiving( const int MAX_DATA_SIZE, const char* IP );
    void closeConnection();

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
    bool sendData( Evento* e );
};
#endif
