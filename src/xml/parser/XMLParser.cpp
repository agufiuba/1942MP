#include "XMLParser.h"
#include <locale>
#include <iostream>
#include <regex>

#include "../../logger/Logger.h"
#include "../../utils/Defaults.h"
#include "../../utils/K.h"

using namespace tinyxml2;
using namespace std;

ServerConf* XMLParser::parseServerConf(string fn) {
  Logger* l = Logger::instance();
  XMLDocument doc;
  int maxClients = Defaults::serverMaxClients;
  int port = Defaults::serverPort;
  bool createFile = false;
  if (!doc.LoadFile(fn.c_str())) {
    l->info("Cargando configuracion desde el archivo " + fn);
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
	l->warn("Cargando server#maxClients por defecto");
      XMLElement* portElement;
      parsed = getElement(serverElement, "port", portElement);
      if (parsed)
	parsed = validInt(portElement);
      if (parsed)
	portElement->QueryIntText(&port);
      else
	l->warn("Cargando server#port por defecto");
    } else {
      l->warn("Cargando configuracion de server por defecto");
    }
  } else {
    createFile = true;
    l->error("Cargando archivo");
    l->warn("Cargando configuracion de server por defecto");
  }
  l->info(
      "Configuracion cargada { maxClients: " + to_string(maxClients)
      + ", port: " + to_string(port) + " }");
  ServerConf* sc = new ServerConf(maxClients, port);
  if (createFile) {
    l->info("Creando archivo con configuracion por defecto del server");
    createXML(sc);
  }
  return sc;
}

ClientConf* XMLParser::parseClientConf(string fn) {
  Logger* l = Logger::instance();
  bool createFile = false;
  XMLDocument doc;
  int serverPort = Defaults::serverPort;
  string serverIP = Defaults::serverIP;
  vector<Mensaje*> messages = vector<Mensaje*>();
  if (!doc.LoadFile(fn.c_str())) {
    l->info("Cargando configuracion desde el archivo " + fn);
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
	l->warn("Cargando client#serverIP por defecto");
      XMLElement* serverPortElement;
      parsed = getElement(clientElement, "serverPort", serverPortElement);
      if (parsed)
	parsed = validInt(serverPortElement);
      if (parsed)
	serverPortElement->QueryIntText(&serverPort);
      else
	l->warn("Cargando client#serverPort por defecto");
      messages = parseMsgs(clientElement);
    } else {
      l->warn("Cargando configuracion de client por defecto");
    }
  } else {
    l->error("Cargando archivo");
    createFile = true;
    Mensaje* msg = new Mensaje();
    strcpy(msg->id, Defaults::generateMsgID().c_str());
    strcpy(msg->tipo, Defaults::type);
    strcpy(msg->valor, Defaults::msgValue.c_str());
    messages.push_back(msg);
    l->warn("Cargando configuracion de client por defecto");
  }
  l->info(
      "Configuracion cargada { serverIP: " + serverIP + ", serverPort: "
      + to_string(serverPort) + " }");
  ClientConf* cc = new ClientConf(serverIP, serverPort, messages);
  if (createFile) {
    l->info("Creando archivo con configuracion por defecto del client");
    createXML(cc);
  }
  return cc;
}

Mensaje* XMLParser::parseMsg(XMLElement* msg) {
  Logger* l = Logger::instance();
  l->info("Leyendo message");
  XMLElement* idElement;
  Mensaje* rmsg;
  bool parsed = getElement(msg, "id", idElement);
  if (parsed) {
    char type[7];
    XMLElement* typeElement;
    parsed = getElement(msg, "type", typeElement);
    locale loc;
    if (parsed) {
      strcpy(type, typeElement->GetText());
    for(int i = 0; i < 7; i++) {
      type[i] = toupper(type[i], loc); 
    }
      if (strcmp(type, K::typeString) == 0
	  || strcmp(type, K::typeInt) == 0
	  || strcmp(type, K::typeDouble) == 0
	  || strcmp(type, K::typeChar) == 0) {
	XMLElement* valueElement;
	parsed = getElement(msg, "value", valueElement);
	if (parsed) {
	  rmsg = new Mensaje();
	  strcpy(rmsg->id, idElement->GetText());
	  strcpy(rmsg->tipo, type);
	  strcpy(rmsg->valor, valueElement->GetText());
	}
      } else
	parsed = false;
    }
  }
  if (!parsed) {
    rmsg = new Mensaje();
    strcpy(rmsg->id, Defaults::generateMsgID().c_str());
    strcpy(rmsg->tipo, Defaults::type);
    strcpy(rmsg->valor, Defaults::msgValue.c_str());
    l->warn("Cargando message por defecto");
  }
  l->info(
      "Message leido { id: " + string(rmsg->id) + ", type: "
      + string(rmsg->tipo) + ", value: " + string(rmsg->valor)
      + " }");
  return rmsg;
}

vector<Mensaje*> XMLParser::parseMsgs(XMLElement* e) {
  Logger* l = Logger::instance();
  l->info("Leyendo lista de messages");
  vector<Mensaje*> msgs;
  XMLElement* msgsElement;
  bool parsed = getElement(e, "messages", msgsElement);
  if (parsed) {
    for (XMLElement* msgElement = msgsElement->FirstChildElement("message");
	msgElement != NULL;
	msgElement = msgElement->NextSiblingElement("message")) {
      msgs.push_back(parseMsg(msgElement));
    }
  } else {
    l->warn("No se encontraron messages");
    l->warn("Agregando message por defecto");
    Mensaje* rmsg = new Mensaje();
    strcpy(rmsg->id, Defaults::generateMsgID().c_str());
    strcpy(rmsg->tipo, Defaults::type);
    strcpy(rmsg->valor, Defaults::msgValue.c_str());
    msgs.push_back(rmsg);
  }
  return msgs;
}

bool XMLParser::getElement(XMLElement* root, string en, XMLElement*& e) {
  Logger* l = Logger::instance();
  e = root->FirstChildElement(en.c_str());
  if (e)
    return true;
  else {
    l->error("Elemento '" + en + "' no encontrado");
    return false;
  }
}

bool XMLParser::getElement(XMLDocument* root, string en, XMLElement*& e) {
  Logger* l = Logger::instance();
  e = root->FirstChildElement(en.c_str());
  if (e)
    return true;
  else {
    l->error("Elemento '" + en + "' no encontrado");
    return false;
  }
}

bool XMLParser::validInt(XMLElement* e) {
  Logger* l = Logger::instance();
  const char* rstr = "^[1-9]\\d*$";
  regex r = regex(rstr);
  bool match = regex_match(e->GetText(), r);
  if (!match)
    l->error(strcat("Parseando a `int` desde ", e->Name()));
  return match;
}

bool XMLParser::validInt(XMLElement* e, string end) {
  Logger* l = Logger::instance();
  const char* rstr = "^[1-9]\\d*$";
  regex r = regex(rstr);
  bool match = regex_match(e->GetText(), r);
  if (!match)
    l->error("Parseando a `int` desde " + end);
  return match;
}

bool XMLParser::validIP(const char* ip) {
  Logger* l = Logger::instance();
  const char* rstr =
    "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
  regex r = regex(rstr);
  bool match = regex_match(ip, r);
  if (!match)
    l->error("Parseando `IP` desde client#serverIP");
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

void XMLParser::createXML(ClientConf* cc) {
  XMLDocument doc;
  XMLElement* ce = doc.NewElement("client");
  XMLElement* sipe = doc.NewElement("serverIP");
  XMLElement* spe = doc.NewElement("serverPort");
  XMLElement* messagese = doc.NewElement("messages");
  XMLElement* messagee = doc.NewElement("message");
  XMLElement* messageide = doc.NewElement("id");
  XMLElement* messagetypee = doc.NewElement("type");
  XMLElement* messagevaluee = doc.NewElement("value");
  XMLText* sip = doc.NewText(cc->getServerIP().c_str());
  XMLText* sp = doc.NewText(to_string(cc->getServerPort()).c_str());
  XMLText* messageid = doc.NewText(cc->getMessages()[0]->id);
  XMLText* messagetype = doc.NewText(cc->getMessages()[0]->tipo);
  XMLText* messagevalue = doc.NewText(cc->getMessages()[0]->valor);
  sipe->LinkEndChild(sip);
  spe->LinkEndChild(sp);
  messageide->LinkEndChild(messageid);
  messagetypee->LinkEndChild(messagetype);
  messagevaluee->LinkEndChild(messagevalue);
  messagee->LinkEndChild(messageide);
  messagee->LinkEndChild(messagetypee);
  messagee->LinkEndChild(messagevaluee);
  messagese->LinkEndChild(messagee);
  ce->LinkEndChild(sipe);
  ce->LinkEndChild(spe);
  ce->LinkEndChild(messagese);
  doc.LinkEndChild(ce);
  doc.SaveFile("default-client-conf.xml");
}
