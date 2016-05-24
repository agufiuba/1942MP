#ifndef SRC_XML_CONF_CLIENTCONF_H_
#define SRC_XML_CONF_CLIENTCONF_H_

#include <vector>

#include "../../models/msg/Msg.h"
#include "../../libs/mensaje/mensaje.h"

class ClientConf {
public:
	ClientConf(string, int, vector<Mensaje*>);
	string getServerIP();
	int getServerPort();
	vector<Mensaje*> getMessages();
	virtual ~ClientConf();
private:
	string serverIP;
	int serverPort;
	vector<Mensaje*> messages;
};

#endif
