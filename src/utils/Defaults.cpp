#include "Defaults.h"

#include "K.h"

const int Defaults::serverMaxClients = 5;
const int Defaults::serverPort = 8080;
const char Defaults::type[7] = "STRING";
const char* Defaults::serverIP = "127.0.0.1";
const string Defaults::msgValue = "HW!";
unsigned short int Defaults::msgIDCounter = 1;
string Defaults::generateMsgID() {
	msgIDCounter++;
	return "DID" + to_string(msgIDCounter - 1);
}
