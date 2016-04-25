#include "Client.h"
#include "../../xml/parser/XMLParser.h"

Client::Client( const char* configFileName ) {
  this->connected = false;
  this->log = Logger::instance();
  this->config = XMLParser::parseClientConf( configFileName ); 
}

Client::~Client() {}

void Client::connect() {

}
