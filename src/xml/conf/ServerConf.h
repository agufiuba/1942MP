#ifndef SRC_XML_CONF_SERVERCONF_H_
#define SRC_XML_CONF_SERVERCONF_H_

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
