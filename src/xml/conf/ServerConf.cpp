#include "ServerConf.h"

ServerConf::ServerConf(int maxClients, int port) {
    this->maxClients = maxClients;
    this->port = port;
}

int ServerConf::getPort() {
    return this->port;
}

int ServerConf::getMaxClients() {
    return this->maxClients;
}

ServerConf::~ServerConf() {}
