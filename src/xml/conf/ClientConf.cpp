#include "ClientConf.h"

ClientConf::ClientConf(const char* serverIP, int serverPort, vector<Msg*> msgs) {
	this->serverIP = serverIP;
	this->serverPort = serverPort;
	this->messages = msgs;
}

const char* ClientConf::getServerIP() {
	return this->serverIP;
}

int ClientConf::getServerPort() {
	return this->serverPort;
}

vector<Msg*> ClientConf::getMessages() {
	return this->messages;
}

ClientConf::~ClientConf() {
}

