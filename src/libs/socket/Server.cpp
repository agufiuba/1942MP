#include "Server.h"
#include "../../xml/parser/XMLParser.h"

Server::Server( const char* configFileName ) {
  this->socketFD = 0;
  this->listening = false;
  this->connected = false;
  this->logger = Logger::instance();
  this->config = XMLParser::parseServerConf( configFileName );
}

Server::~Server() {

}
