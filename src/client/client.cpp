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
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"

using namespace std;

int gfd = 0;
bool connected = false;

Menu clientMenu("Menu de opciones del Cliente");
const int MSG_QUANTITY = 4;
string msgQueue[MSG_QUANTITY] = { "hola", "mundo", "chau", "gente" };

void closeConnection() {
  close(gfd);
  connected = false;
  cout << endl << warning("Se cerro la conexion con el servidor.") << endl;
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
  const char* IP = "127.0.0.1";

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

  // while(connected) {
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
  //}
}

void sendData(string data, int dataLength) {
  if (send(gfd, data.c_str(), dataLength, 0) == -1) {
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

void sendMsg(int id) {
  if (!connected) {
    cout << endl
      << warning("Para mandar un mensaje debe estar conectado al servidor.")
      << endl;
    return;
  }

  string data = msgQueue[id];
  int dataLength = msgQueue[id].length();
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

  for (int i = 0; i < MSG_QUANTITY; i++) {
    sendMsg(i);
    if (i != MSG_QUANTITY - 1) {
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
