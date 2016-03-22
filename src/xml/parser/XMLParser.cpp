#include "XMLParser.h"

#include <iostream>
#include <regex>

#include "../../utils/Colors.h"
#include "../../utils/Defaults.h"

using namespace tinyxml2;
using namespace std;

ServerConf* XMLParser::parseServerConf(string fn) {
	XMLDocument doc;
	int maxClients = Defaults::serverMaxClients;
	int port = Defaults::serverPort;
	bool createFile = false;
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
				cout << "Warn - Loading default server maxClients" << endl;
			XMLElement* portElement;
			parsed = getElement(serverElement, "port", portElement);
			if (parsed)
				parsed = validInt(portElement);
			if (parsed)
				portElement->QueryIntText(&port);
			else
				cout << "Warn - Loading default server port" << endl;
		} else {
			cout << "Warn - Loading default server configuration" << endl;
		}
	} else {
		createFile = true;
		cout << "Error - Loading file" << endl;
		cout << "Warn - Loading default server configuration" << endl;
	}
	cout << "Info - Loaded configuration { maxClients: " << maxClients
			<< ", port: " << port << " }" << endl;
	ServerConf* sc = new ServerConf(maxClients, port);
	if (createFile) {
		cout << "Info - Saving default server configuration XML file" << endl;
		createXML(sc);
	}
	return sc;
}

ClientConf* XMLParser::parseClientConf(string fn) {
	XMLDocument doc;
	int serverPort = Defaults::serverPort;
	const char* serverIP = Defaults::serverIP;
	vector<Msg*> messages = vector<Msg*>();
	if (!doc.LoadFile(fn.c_str())) {
		cout << "Info - Loading client configuration from file " << fn << endl;
		XMLElement* clientElement;
		if (getElement(&doc, "client", clientElement)) {
			XMLElement* serverIPElement;
			bool parsed = getElement(clientElement, "serverIP",
					serverIPElement);
			if (parsed)
				parsed = validIP(serverIPElement->GetText());
			if (parsed)
				serverIP = serverIPElement->GetText();
			else
				cout << "Warn - Loading default client server IP" << endl;
			XMLElement* serverPortElement;
			parsed = getElement(clientElement, "serverPort", serverPortElement);
			if (parsed)
				parsed = validInt(serverPortElement);
			if (parsed)
				serverPortElement->QueryIntText(&serverPort);
			else
				cout << "Warn - Loading default client server port" << endl;
			messages = parseMsgs(clientElement);
		} else {
			cout << "Warn - Loading default client configuration" << endl;
		}
	} else {
		cout << "Error - Loading file" << endl;
		cout << "Warn - Loading default client configuration" << endl;
	}
	cout << "Info - Loaded configuration { serverIP: " << serverIP
			<< ", serverPort: " << serverPort << " }" << endl;
	return new ClientConf(serverIP, serverPort, messages);
}

Msg* XMLParser::parseMsg(XMLElement* msg) {
	cout << "Info - Reading message" << endl;

	// Validar id
	string id = msg->FirstChildElement("id")->GetText();

	int type = Defaults::defaultType;
	XMLElement* typeElement = msg->FirstChildElement("type");
	if (validInt(typeElement, ("message " + id + "#type").c_str()))
		typeElement->QueryIntText(&type);
	else
		cout << "Warn - Loading default message type: String" << endl;

	// Validar valor segun el tipo
	string value = msg->FirstChildElement("value")->GetText();

	cout << "Info - Read message { id: " << id << ", type: " << type
			<< ", value: " << value << " }" << endl;
	return new Msg(id, type, value);
}

vector<Msg*> XMLParser::parseMsgs(XMLElement* e) {
	cout << "Info - Reading list of messages" << endl;
	vector<Msg*> msgs;
	for (XMLElement* msgElement = e->FirstChildElement("message");
			msgElement != NULL;
			msgElement = msgElement->NextSiblingElement("message")) {
		msgs.push_back(parseMsg(msgElement));
	}
	return msgs;
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

bool XMLParser::validInt(XMLElement* e, const char* end) {
	const char* rstr = "^[1-9]\\d*$";
	regex r = regex(rstr);
	bool match = regex_match(e->GetText(), r);
	if (!match)
		cout << "Error - Parsing `int` from " << end << endl;
	return match;
}

bool XMLParser::validIP(const char* ip) {
	const char* rstr =
			"^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
	regex r = regex(rstr);
	bool match = regex_match(ip, r);
	if (!match)
		cout << "Error - Parsing `IP` from serverIP" << endl;
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
