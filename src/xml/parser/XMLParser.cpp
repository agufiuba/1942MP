#include "XMLParser.h"

#include <iostream>
#include <regex>

#include "../../utils/Defaults.h"

using namespace tinyxml2;
using namespace std;

ServerConf* XMLParser::parseServerConf(string fn) {
	XMLDocument doc;
	int maxClients = Defaults::serverMaxClients;
	bool createFile = false;
	int port = Defaults::serverPort;
	if (!doc.LoadFile(fn.c_str())) {
		cout << "Info - Loading server configuration from file " << fn << endl;
		XMLElement* serverElement;
		if (getElement(&doc, "server", serverElement)) {
			XMLElement* maxClientsElement;
			bool parsed = getElement(serverElement, "maxClients",
					maxClientsElement);
			if (parsed)
				parsed = validInt(maxClientsElement);
			if (parsed)
				maxClientsElement->QueryIntText(&maxClients);
			else
				cout << "Info - Loading default server maxClients" << endl;
			XMLElement* portElement;
			parsed = getElement(serverElement, "port", portElement);
			if (parsed)
				parsed = validInt(portElement);
			if (parsed)
				portElement->QueryIntText(&port);
			else
				cout << "Info - Loading default server port" << endl;
		} else {
			cout << "Info - Loading default server configuration" << endl;
		}
	} else {
		createFile = true;
		cout << "Error - Loading file" << endl;
		cout << "Info - Loading default server configuration" << endl;
	}
	cout << "Info - Loaded configuration { maxClients: " << maxClients
			<< ", port: " << port << " }" << endl;
	ServerConf* sc = new ServerConf(maxClients, port);
	if (createFile) {
		createXML(sc);
		cout << "Info - Saving default server configuration XML file" << endl;
	}
	return sc;
}

bool XMLParser::getElement(XMLElement* root, const char* en, XMLElement*& e) {
	e = root->FirstChildElement(en);
	if (e)
		return true;
	else {
		cout << "Error - Element '" << en << "' not found" << endl;
		return false;
	}
}

bool XMLParser::getElement(XMLDocument* root, const char* en, XMLElement*& e) {
	e = root->FirstChildElement(en);
	if (e)
		return true;
	else {
		cout << "Error - Element '" << en << "' not found" << endl;
		return false;
	}
}

bool XMLParser::validInt(XMLElement* e) {
	const char* rstr = "^[1-9]\\d*$";
	regex r = regex(rstr);
	bool match = regex_match(e->GetText(), r);
	if (!match)
		cout << "Error - Parsing `int` from " << e->Name() << endl;
	return match;
}

void XMLParser::createXML(ServerConf* sc) {
	XMLDocument doc;
	XMLElement* se = doc.NewElement("server");
	XMLElement* mce = doc.NewElement("maxClients");
	XMLElement* pe = doc.NewElement("port");
	XMLText* mc = doc.NewText((to_string(sc->getMaxClients())).c_str());
	XMLText* p = doc.NewText((to_string(sc->getPort())).c_str());
	mce->LinkEndChild(mc);
	pe->LinkEndChild(p);
	se->LinkEndChild(mce);
	se->LinkEndChild(pe);
	doc.LinkEndChild(se);
	doc.SaveFile("default-server-conf.xml");
}
