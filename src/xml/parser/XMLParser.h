#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <vector>

#include "../../libs/tinyxml2.h"
#include "../../libs/mensaje/mensaje.h"
#include "../conf/ServerConf.h"
#include "../conf/ClientConf.h"

using namespace tinyxml2;
using namespace std;

class XMLParser {
public:
	static ServerConf* parseServerConf(string);
    static ClientConf* parseClientConf(string);
private:
	static bool getElement(XMLElement*, string, XMLElement*&);
	static bool getElement(XMLDocument*, string, XMLElement*&);
	static bool validInt(XMLElement*);
	static bool validInt(XMLElement*, string);
    static bool validIP(const char*);
	static Mensaje* parseMsg(XMLElement*);
	static vector<Mensaje*> parseMsgs(XMLElement*);
	static void createXML(ServerConf*);
};

#endif
