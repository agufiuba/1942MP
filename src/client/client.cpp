// client.cpp
#include <sys/socket.h> /* socket() */
#include <sys/types.h> /* socket() */
#include <netinet/in.h> /* IPv4 & IPv6 presentation strings max.length constants (INET6?_ADDRSTRLEN) */
#include <arpa/inet.h> /* inet_aton(,) */
#include <netdb.h> /* hostent */
#include <unistd.h> /* close() */
#include <cstdlib> /* exit() */
#include <cstring> /* memset(), bzero() */
#include <iostream>
#include <thread>
#include <string>
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"
#include "../logger/Logger.h"
#include "../xml/parser/XMLParser.h"
#include "../xml/conf/ClientConf.h"

using namespace std;

int gfd = 0;
bool connected = false;
ClientConf* cc;
Logger* logger = Logger::instance();

Menu clientMenu("Menu de opciones del Cliente");
const int MSG_QUANTITY = 4;
string msgQueue[MSG_QUANTITY] = { "hola", "mundo", "chau", "gente" };

void closeConnection() {
  close(gfd);
  connected = false;
  logger->warn("Se cerro la conexion con el servidor.");
  cout << endl << warning("Se cerro la conexion con el servidor.") << endl;
}

void receiving(int sfd, char buf[], const int MAX_DATA_SIZE, const char * IP){
	int numBytesRead = 1;
	while (numBytesRead != 0 && numBytesRead != -1) {
	  if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
	  	logger->error("Falla al recibir el Msj");
	    exit(-1);
	  }
	  if (numBytesRead) {
	    buf[numBytesRead] = '\0';
	    cout << endl << "Mensaje del servidor: " << string(buf) <<endl;
	    logger->info("Mensaje del servidor: " + string(buf));
	  } else {
	  	logger->warn("Se perdio la conexion con el servidor.");
	  	cout << endl << warning("Se perdio la conexion con el servidor.") << endl;
	    connected = false;
	    close(sfd);
	  }
	}
}

void srvConnect() {
  if (connected) {
  	logger->warn("Ya hay una conexion activa");
  	cout << "Ya hay una conexion activa" << endl;
  	return;
  }


//  const int PORT = 5340;
  const int MAX_DATA_SIZE = 10000; /* Max. number of bytes for recv */
  int sfd, numBytesRead;
  char buf[MAX_DATA_SIZE]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */
//  const char* IP = "127.0.0.1";

  /* Create socket */
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    logger->error("Falla en el Socket");
    exit(-1);
  }

  gfd = sfd;

  server.sin_family = AF_INET;
  server.sin_port = htons(cc->getServerPort());
  if ((inet_aton(cc->getServerIP().c_str(), &server.sin_addr)) == 0) {
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
      	logger->error("Error de conexion. Reintentando conexion...");
      	cout<< "Error de conexion, Reintentando conexion..." << endl;
	usleep(5000000);
      } else {
      	logger->warn("No se pudo establecer una conexion con el servidor: "
      			+cc->getServerIP()+" Por favor intente nuevamente mas tarde.");
      	cout << endl << warning("No se pudo establecer una conexion con el servidor: ")
	     << cc->getServerIP() << endl << "Por favor intente nuevamente mas tarde." << endl;
	return;
      }
    } else {
      connected = true;
    }
  }
  if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
    logger->error("Error al recibir Msj");
  	exit(-1);
  }
  if (numBytesRead) {
  	logger->info("Se establecio una conexion con: "+cc->getServerIP());
    cout << endl << notice("Se establecio una conexion con: ") << cc->getServerIP() << endl;
    buf[numBytesRead] = '\0';
    logger->info("Mensaje del servidor: " +string(buf) );
    cout << "Mensaje del servidor: " << buf << endl;
  } else {
    logger->warn("Se perdio la conexion con el servidor.");
    cout << endl << warning("Se perdio la conexion con el servidor.") << endl;
    connected = false;
    close(sfd);
  }

  std::thread tReceiving (receiving, sfd, buf, MAX_DATA_SIZE, cc->getServerIP().c_str());
  tReceiving.detach();
}

void sendData(string data, int dataLength) {
  if (send(gfd, data.c_str(), dataLength, 0) == -1) {
  	logger->error("Error al enviar msj al servidor");
  }
}

void srvDisconnect() {
  if (connected) {
    closeConnection();
  } else {
    logger->warn("No hay una conexion activa");
  	cout << endl << warning("No hay una conexion activa") << endl;
  }
}

void exitPgm() {
  if (connected)
    closeConnection();
  logger->warn("Cerrando el cliente.");
  cout << endl << warning("Cerrando el cliente...") << endl;
  exit(1);
}

void sendMsg(int id) {
  if (!connected) {
  	logger->warn("Intento de envio de msj sin estar conectado al servidor.");
    cout << endl
      << warning("Para mandar un mensaje debe estar conectado al servidor.")
      << endl;
    return;
  }

  string data = msgQueue[id];
  int dataLength = msgQueue[id].length();
  logger->info("Se envio: "+data+" al servidor");
  cout << endl << "Se envio '" << notice(data) << "' al servidor" << endl;
  sendData(data, dataLength);
}

void addMsgOptions() {
  for (int i = 0; i < MSG_QUANTITY; i++) {
    string optionName = "Enviar mensaje " + to_string(i) + " ";
    clientMenu.addOption(optionName, sendMsg, i);
  }
}

void cycle() {
  int timeout = 0;
  cout << "Ingrese duracion (en milisegundos): ";
  cin >> timeout;
  logger->info("Se corre ciclar en "+to_string(timeout)+" milisegundos.");
  for (int i = 0; i < MSG_QUANTITY; i++) {
    sendMsg(i);
    if (i != MSG_QUANTITY - 1) {
      usleep(timeout * 1000);
    }
  }
  usleep(5000);/*agregado solo para que reciba el ultimo mensaje del servidor, antes de hacer display
  							del menu*/
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
