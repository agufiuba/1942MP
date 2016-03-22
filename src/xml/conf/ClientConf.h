#ifndef SRC_XML_CONF_CLIENTCONF_H_
#define SRC_XML_CONF_CLIENTCONF_H_

#include <vector>

#include "../../models/msg/Msg.h"

class ClientConf {
public:
	ClientConf(const char*, int, vector<Msg*>);
	const char* getServerIP();
	int getServerPort();
	vector<Msg*> getMessages();
	virtual ~ClientConf();
private:
	const char* serverIP;
	int serverPort;
	vector<Msg*> messages;
};

#endif
