#ifndef SRC_XML_CONF_CLIENTCONF_H_
#define SRC_XML_CONF_CLIENTCONF_H_

#include <vector>

#include "../../models/msg/Msg.h"

class ClientConf {
public:
	ClientConf(string, int, vector<Msg*>);
	string getServerIP();
	int getServerPort();
	vector<Msg*> getMessages();
	virtual ~ClientConf();
private:
	void serialize(Mensaje mensaje, char* dataSerialize);
	string serverIP;
	int serverPort;
	vector<Msg*> messages;
};

#endif
