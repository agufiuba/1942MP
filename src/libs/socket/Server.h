#ifndef SERVER_H
#define SERVER_H
#include "../logger/Logger.h"
#include "../../xml/conf/ServerConf.h"

class Server {
  private:
    int socketFD;
    bool listening;
    bool connected;
    Logger* logger;
    ServerConf* config;

  public:
    Server( const char* configFileName );
    ~Server();
};
#endif
