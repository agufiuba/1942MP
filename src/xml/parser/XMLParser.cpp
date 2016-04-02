#include "XMLParser.h"

#include <iostream>
#include <regex>

#include "../../logger/Logger.h"
#include "../../utils/Defaults.h"

using namespace tinyxml2;
using namespace std;

ServerConf* XMLParser::parseServerConf(string fn) {
	Logger* l = Logger::instance();
	XMLDocument doc;
	int maxClients = Defaults::serverMaxClients;
	int port = Defaults::serverPort;
	bool createFile = false;
	if (!doc.LoadFile(fn.c_str())) {
		l->info("Loading server configuration from file " + fn);
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
				l->warn("Loading default server maxClients");
			XMLElement* portElement;
			parsed = getElement(serverElement, "port", portElement);
			if (parsed)
				parsed = validInt(portElement);
			if (parsed)
				portElement->QueryIntText(&port);
			else
				l->warn("Loading default server port");
		} else {
			l->warn("Loading default server configuration");
		}
	} else {
		createFile = true;
		l->error("Loading file");
		l->warn("Loading default server configuration");
	}
	l->info(
			"Loaded configuration { maxClients: " + to_string(maxClients)
					+ ", port: " + to_string(port) + " }");
	ServerConf* sc = new ServerConf(maxClients, port);
	if (createFile) {
		l->info("Saving default server configuration XML file");
		createXML(sc);
	}
	return sc;
}

ClientConf* XMLParser::parseClientConf(string fn) {
	Logger* l = Logger::instance();
	XMLDocument doc;
	int serverPort = Defaults::serverPort;
	string serverIP = Defaults::serverIP;
	vector<Msg*> messages = vector<Msg*>();
	if (!doc.LoadFile(fn.c_str())) {
		l->info("Loading client configuration from file " + fn);
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
				l->warn("Loading default client server IP");
			XMLElement* serverPortElement;
			parsed = getElement(clientElement, "serverPort", serverPortElement);
			if (parsed)
				parsed = validInt(serverPortElement);
			if (parsed)
				serverPortElement->QueryIntText(&serverPort);
			else
				l->warn("Loading default client server port");
			messages = parseMsgs(clientElement);
		} else {
			l->warn("Loading default client configuration");
		}
	} else {
		l->error("Loading file");
		l->warn("Loading default client configuration");
	}
	l->info(
			"Loaded configuration { serverIP: " + serverIP + ", serverPort: "
					+ to_string(serverPort) + " }");
	return new ClientConf(serverIP, serverPort, messages);
}

Msg* XMLParser::parseMsg(XMLElement* msg) {
	Logger* l = Logger::instance();
	l->info("Reading message");

	// Validar id
	string id = msg->FirstChildElement("id")->GetText();

	int type = Defaults::defaultType;
	XMLElement* typeElement = msg->FirstChildElement("type");
	if (validInt(typeElement, ("message " + id + "#type").c_str()))
		typeElement->QueryIntText(&type);
	else
		l->warn("Loading default message type: String");

	// Validar valor segun el tipo
	string value = msg->FirstChildElement("value")->GetText();

	l->info(
			"Read message { id: " + id + ", type: " + to_string(type)
					+ ", value: " + value + " }");
	return new Msg(id, type, value);
}

vector<Msg*> XMLParser::parseMsgs(XMLElement* e) {
	Logger* l = Logger::instance();
	l->info("Reading list of messages");
	vector<Msg*> msgs;
	for (XMLElement* msgElement = e->FirstChildElement("message");
			msgElement != NULL;
			msgElement = msgElement->NextSiblingElement("message")) {
		msgs.push_back(parseMsg(msgElement));
	}
	return msgs;
}

bool XMLParser::getElement(XMLElement* root, string en, XMLElement*& e) {
	Logger* l = Logger::instance();
	e = root->FirstChildElement(en.c_str());
	if (e)
		return true;
	else {
		l->error("Element '" + en + "' not found");
		return false;
	}
}

bool XMLParser::getElement(XMLDocument* root, string en, XMLElement*& e) {
	Logger* l = Logger::instance();
	e = root->FirstChildElement(en.c_str());
	if (e)
		return true;
	else {
		l->error("Element '" + en + "' not found");
		return false;
	}
}

bool XMLParser::validInt(XMLElement* e) {
	Logger* l = Logger::instance();
	const char* rstr = "^[1-9]\\d*$";
	regex r = regex(rstr);
	bool match = regex_match(e->GetText(), r);
	if (!match)
		l->error(strcat("Parsing `int` from ", e->Name()));
	return match;
}

bool XMLParser::validInt(XMLElement* e, string end) {
	Logger* l = Logger::instance();
	const char* rstr = "^[1-9]\\d*$";
	regex r = regex(rstr);
	bool match = regex_match(e->GetText(), r);
	if (!match)
		l->error("Parsing `int` from " + end);
	return match;
}

bool XMLParser::validIP(const char* ip) {
	Logger* l = Logger::instance();
	const char* rstr =
			"^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
	regex r = regex(rstr);
	bool match = regex_match(ip, r);
	if (!match)
		l->error("Parsing `IP` from serverIP");
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
