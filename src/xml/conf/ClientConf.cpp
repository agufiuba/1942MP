#include "ClientConf.h"

ClientConf::ClientConf(string serverIP, int serverPort, vector<Mensaje*> msgs) {
	this->serverIP = serverIP;
	this->serverPort = serverPort;
	this->messages = msgs;
}

string ClientConf::getServerIP() {
	return this->serverIP;
}

int ClientConf::getServerPort() {
	return this->serverPort;
}

vector<Mensaje*> ClientConf::getMessages() {
	return this->messages;
}

ClientConf::~ClientConf() {
}
