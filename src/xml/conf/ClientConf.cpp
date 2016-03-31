#include "ClientConf.h"

ClientConf::ClientConf(string serverIP, int serverPort, vector<Msg*> msgs) {
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

vector<Msg*> ClientConf::getMessages() {
	return this->messages;
}

ClientConf::~ClientConf() {
}

