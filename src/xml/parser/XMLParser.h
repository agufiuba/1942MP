#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <vector>

#include "../../libs/tinyxml2.h"
#include "../../models/msg/Msg.h"
#include "../conf/ServerConf.h"
#include "../conf/ClientConf.h"

using namespace tinyxml2;
using namespace std;

class XMLParser {
public:
	static ServerConf* parseServerConf(string);
    static ClientConf* parseClientConf(string);
private:
	static bool getElement(XMLElement*, const char*, XMLElement*&);
	static bool getElement(XMLDocument*, const char*, XMLElement*&);
	static bool validInt(XMLElement*);
	static bool validInt(XMLElement*, const char*);
    static bool validIP(const char*);
	static Msg* parseMsg(XMLElement*);
	static vector<Msg*> parseMsgs(XMLElement*);
	static void createXML(ServerConf*);
};

#endif
