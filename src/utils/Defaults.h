#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <string>

using namespace std;

class Defaults {
public:
	const static int serverMaxClients;
	const static int serverPort;
	const static char type[7];
	const static char* serverIP;
	const static string msgValue;
	static string generateMsgID();
private:
	static unsigned short int msgIDCounter;
};

#endif
