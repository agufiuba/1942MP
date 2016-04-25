#ifndef CLIENT_H
#define CLIENT_H
#include "../logger/Logger.h"
#include "../../xml/conf/ClientConf.h"

class Client {
  private:
    bool connected;
    Logger* log;
    static const int MAX_UNREACHABLE_TIME = 5;
    ClientConf* config;

  public:
    Client( const char* configFileName );
    void connect();
    ~Client();
};
#endif
