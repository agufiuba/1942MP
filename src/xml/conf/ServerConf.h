#ifndef XMLSERVER_H
#define XMLSERVER_H

class ServerConf {
public:
    ServerConf(int, int);
    int getPort();
    int getMaxClients();
    virtual ~ServerConf();
private:
    int port;
    int maxClients;
};

#endif
