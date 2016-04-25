#ifndef CLIENT_H
#define CLIENT_H
#include "../logger/Logger.h"
#include "../../xml/conf/ClientConf.h"

class Client {
  private:
    int socketFD;
    bool connected;
    bool received;
    Logger* logger;
    static const int MAX_UNREACHABLE_TIME = 5;
    ClientConf* config;
    void checkAliveSend();
    void receiving( const int MAX_DATA_SIZE, const char* IP );

  public:
    Client( const char* configFileName );
    void connectToServer();
    ~Client();
};
#endif
