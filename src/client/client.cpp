#include "../libs/socket/sock_dep.h" /* socket dependencies */
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"
#include "../logger/Logger.h"
#include "../xml/parser/XMLParser.h"
#include "../xml/conf/ClientConf.h"
#include <thread>
#include <mutex>
#include <iostream>
#define DEBUG 1
#include "../libs/debug/dg_msg.h"
//#include "../models/msg/Mensaje.h"
#include "../models/serialize/Serialize.h"

using namespace std;

int gfd = 0;
bool connected = false;
ClientConf* cc;
Logger* logger = Logger::instance();
mutex theMutex;
Menu clientMenu("Menu de opciones del Cliente");
const int MENS_QUANTITY = 2;
const int MSG_QUANTITY = 4	;
string msgQueue[MSG_QUANTITY] = { "hola", "mundo", "chau", "gente" };
Mensaje mensajeQueue[MENS_QUANTITY];

void closeConnection() {
  close(gfd);
  connected = false;
  logger->warn(CONNECTION_CLOSE);
  DEBUG_WARN(CONNECTION_CLOSE);
}

void receiving(int sfd, char buf[], const int MAX_DATA_SIZE, const char *IP){
  int numBytesRead = 1;
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

  while (connected) {

		// seteo el timeout de recepcion de mensajes
		if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout)) < 0) {
			cout << "Error sockopt" << endl;
			exit(1);
		}

    if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
			if (!connected) {
				connected = false;
				close(sfd);
			}
    }
    if (numBytesRead>0) {
      buf[numBytesRead] = '\0';
      string recvMsg = string(buf);
      logger->info(SERVER_MSG(recvMsg));
      DEBUG_PRINT(SERVER_MSG(recvMsg));
    }
    if (numBytesRead == 0){
			logger->warn(CONNECTION_LOST);
			DEBUG_WARN(CONNECTION_LOST);
			connected = false;
			close(sfd);
    }
  }
}

void srvConnect() {
  if (connected) {
    logger->warn(CONNECTION_ACTIVE);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_ACTIVE);
    theMutex.unlock();
    return;
  }

  const int MAX_DATA_SIZE = 10000; /* Max. number of bytes for recv */
  int sfd, numBytesRead; /* socketFD, bytes read count */
  char buf[MAX_DATA_SIZE]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */
  string serverIP = cc->getServerIP();

  /* Create socket */
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    logger->error(SOCKET_ERROR);
    exit(-1);
  }

  gfd = sfd;

  server.sin_family = AF_INET;
  server.sin_port = htons(cc->getServerPort());
  if ((inet_aton(serverIP.c_str(), &server.sin_addr)) == 0) {
    logger->error("IP invalido");
    exit(-1);
  }

  bzero(&(server.sin_zero), 8);

  /* Connect to server */
  short triesLeft = 3;

  while (!connected && triesLeft) {
    if (connect(sfd, (struct sockaddr*) &server, sizeof(struct sockaddr))
	== -1) {
      triesLeft--;
      /* 5s delay for retry */
      if(triesLeft) {
	logger->error(CONNECTION_RETRY);
	theMutex.lock();
	DEBUG_WARN(CONNECTION_RETRY);
	theMutex.unlock();
	usleep(5000000);
      } else {
	logger->warn(CONNECTION_ERROR(serverIP));
	theMutex.lock();
	DEBUG_PRINT(CONNECTION_ERROR(serverIP));
	theMutex.unlock();
	return;
      }
    } else {
      connected = true;
    }
  }

  // Get server welcome message
  if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
    logger->error(RECV_FAIL);
  }
  if (numBytesRead>0) {
    logger->info(CONNECTION_SUCCESS(serverIP));
    theMutex.lock();
    DEBUG_PRINT(CONNECTION_SUCCESS(serverIP));
    buf[numBytesRead] = '\0';
    string recvMsg = string(buf);
    logger->info(SERVER_MSG(recvMsg));
    DEBUG_PRINT(SERVER_MSG(recvMsg));
    theMutex.unlock();
  } else {
    logger->warn(CONNECTION_LOST);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_LOST);
    theMutex.unlock();
    connected = false;
    close(sfd);
  }

  // Create thread for receiving data from server
  thread tReceiving(receiving, sfd, buf, MAX_DATA_SIZE, serverIP.c_str());
  tReceiving.detach();
}

void srvDisconnect() {
  if (connected) {
    closeConnection();
  } else {
    logger->warn(CONNECTION_NOT_ACTIVE);
    theMutex.lock();
    DEBUG_WARN(CONNECTION_NOT_ACTIVE);
    theMutex.unlock();
  }
}

void exitPgm() {
  if(connected)
    closeConnection();
  logger->warn(CLIENT_CLOSE);
  theMutex.lock();
  DEBUG_WARN(CLIENT_CLOSE);
  theMutex.unlock();
  exit(0);
}

bool sendData(Mensaje data, int dataLength) {

	string dataSerialize = Serialize::serialize(data);
	cout << "Voy a mandar: " << dataSerialize << endl;
  if(send(gfd, dataSerialize.c_str(), dataLength, 0) == -1) {
    logger->error(SEND_FAIL);
    theMutex.lock();
    DEBUG_WARN(SEND_FAIL);
    theMutex.unlock();
    return false;
  }
  return true;
}

bool sendMsg(int id) {
  if(!connected) {
    logger->warn(SEND_CERROR);
    theMutex.lock();
    DEBUG_WARN(SEND_CERROR);
    theMutex.unlock();
    return false;
  }

  Mensaje data = mensajeQueue[id];
  int dataLength = sizeof(mensajeQueue[id]);
  logger->info(SENT_DATA(mensajeQueue[id].id));
  theMutex.lock();
  DEBUG_PRINT(SENT_DATA(mensajeQueue[id].id));
  theMutex.unlock();
  return sendData(data, dataLength);
}

void addMsgOptions() {

	Mensaje msg1;
	msg1.id = "01";
	msg1.tipo = "int";
	msg1.valor = "El valor del presente mensaje es el 01, independiente mente del id que conciden y el tipo de dicho mensaje";

	Mensaje msg2;
	msg2.id = "02";
	msg2.tipo = "string";
	msg2.valor = "Nada";

	mensajeQueue[0] = msg1;
	mensajeQueue[1] = msg2;


  for (int i = 0; i < MENS_QUANTITY; i++) {
    string optionName = "Enviar mensaje " + mensajeQueue[i].id;
    clientMenu.addOption(optionName, sendMsg, i);
  }
}

void cycle() {
  int timeout = 0;
  cout << "Ingrese duracion (en milisegundos): ";
  cin >> timeout;
  logger->info("Se corre ciclar en " + to_string(timeout) + " milisegundos.");
  for (int i = 0; i < MSG_QUANTITY; i++) {
    if(!sendMsg(i)) return;
    if (i != MSG_QUANTITY - 1) {
      usleep(timeout * 1000);
    }
  }
  usleep(5000);/* agregado solo para que reciba el ultimo mensaje del servidor, 
		  antes de hacer display del menu*/
}

int main(int argc, char* argv[]) {
  const char* fileName = argv[1] ? argv[1] : "default-cc.xml";
  cc = XMLParser::parseClientConf(fileName);

  clientMenu.addOption("Conectar", srvConnect);
  clientMenu.addOption("Desconectar", srvDisconnect);
  addMsgOptions();
  clientMenu.addOption("Ciclar", cycle);
  clientMenu.addOption("Salir", exitPgm);

  clientMenu.display();

  return 0;
}
