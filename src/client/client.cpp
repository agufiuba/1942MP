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
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"

using namespace std;

int gfd = 0;
bool connected = false;
const int MSG_COUNT = 2;

typedef struct Mensaje{
  string id;
  string tipo;
  string valor;
} Mensaje;

Mensaje* mejus[MSG_COUNT];

Menu clientMenu("Menu de opciones del Cliente");

void closeConnection() {
  close(gfd);
  connected = false;
  cout << endl << warning("Se cerro la conexion con el servidor.") << endl;
}

void receiving(int sfd, char buf[], const int MAX_DATA_SIZE, const char * IP){
  int numBytesRead = 1;
  while(numBytesRead != 0 && numBytesRead != -1) {
    if((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
      cout << "recv error" << endl;
      exit(-1);
    }
    if(numBytesRead) {
      buf[numBytesRead] = '\0';
      cout << "Mensaje del servidor: " << buf << endl;
    } else {
      cout << endl << warning("Se perdio la conexion con el servidor.") << endl;
      connected = false;
      close(sfd);
    }
  }
}

void srvConnect() {
  if (connected) {
    cout << endl << warning("Ya hay una conexion activa") << endl;
    return;
  }

  const int PORT = 5340;
  const int MAX_DATA_SIZE = 100; /* Max. number of bytes for recv */
  int sfd, numBytesRead;
  char buf[MAX_DATA_SIZE]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */
  const char* IP = "192.168.1.109";

  /* Create socket */
  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cout << "socket error" << endl;
    exit(-1);
  }

  gfd = sfd;

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if ((inet_aton(IP, &server.sin_addr)) == 0) {
    cout << "invalid IP" << endl;
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
	cout << endl << warning("Error de conexion.") << endl
	  << "Reintentando conexion..." << endl;
	usleep(5000000);
      } else {
	cout << endl << warning("No se pudo establecer una conexion con el servidor: ")
	  << IP << endl << "Por favor intente nuevamente mas tarde." << endl;
	return;
      }
    } else {
      connected = true;
    }
  }
  if ((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
    cout << "recv error" << endl;
    exit(-1);
  }
  if (numBytesRead) {
    cout << endl << notice("Se establecio una conexion con: ") << IP << endl;
    buf[numBytesRead] = '\0';
    cout << "Mensaje del servidor: " << buf << endl;
  } else {
    cout << endl << warning("Se perdio la conexion con el servidor.") << endl;
    connected = false;
    close(sfd);
  }

  std::thread tReceiving (receiving, sfd, buf, MAX_DATA_SIZE, IP);
  tReceiving.detach();
}

void sendData(Mensaje data, int dataLength) {
  if (send(gfd, &data, dataLength, 0) == -1) {
    cout << "send error" << endl;
  }
}

void srvDisconnect() {
  if (connected) {
    closeConnection();
  } else {
    cout << endl << warning("No hay una conexion activa") << endl;
  }
}

void exitPgm() {
  if (connected)
    closeConnection();

  cout << endl << warning("Cerrando el cliente...") << endl;
  exit(1);
}

void sendMsg(string id) {  
  if (!connected) {
    cout << endl
      << warning("Para mandar un mensaje debe estar conectado al servidor.")
      << endl;
    return;
  }

  Mensaje* msgToSend;

  for (int i = 0; i < MSG_COUNT; i++) {
    if(mejus[i]->id == id) {
      msgToSend = mejus[i];
      break;
    }
  }

  int dataLength = sizeof(Mensaje); 
  cout << msgToSend;
  cout << endl << "Se envio '" << notice(msgToSend->valor) << "' al servidor" << endl;
  sendData(*msgToSend, dataLength);
}

void addMsgOptions() {
  Mensaje* msg1 = new Mensaje;
  msg1->id = "id8912";
  msg1->tipo = "int";
  msg1->valor = "28";

  Mensaje* msg2 = new Mensaje;
  msg2->id = "id1238";
  msg2->tipo = "string";
  msg2->valor = "kkkeeeeeeeeeeeeyyyyyyyy";

  mejus[0] = msg1;
  mejus[1] = msg2;

  for (int i = 0; i < MSG_COUNT; i++) {
    string optionName = "Enviar mensaje " + mejus[i]->id;
    clientMenu.addOption(optionName, sendMsg, mejus[i]->id);
  }
}

void cycle() {
  int timeout = 0;
  cout << "Ingrese duracion (en milisegundos): ";
  cin >> timeout;

  for (int i = 0; i < MSG_COUNT; i++) {
    sendMsg(mejus[i]->id);
    if (i != MSG_COUNT- 1) {
      usleep(timeout * 1000);
    }
  }
}

int main(int argc, char* argv[]) {
  const char* fileName = argv[1] ? argv[1] : "default.xml";

  clientMenu.addOption("Conectar", srvConnect);
  clientMenu.addOption("Desconectar", srvDisconnect);
  addMsgOptions();
  clientMenu.addOption("Ciclar", cycle);
  clientMenu.addOption("Salir", exitPgm);

  clientMenu.display();

  return 0;
}
