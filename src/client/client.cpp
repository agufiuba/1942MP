// client.cpp
#include <sys/socket.h>
#include <netinet/in.h> /* IPv4 & IPv6 presentation strings max.length constants (INET6?_ADDRSTRLEN) */
#include <arpa/inet.h> /* inet_aton(,) */
#include <netdb.h> /* hostent */
#include <unistd.h> /* close() */
#include <cstdlib> /* exit() */
#include <cstring> /* memset(), bzero() */
#include <iostream>
#include "../libs/menu/Menu.h"

using namespace std;

int gfd = 0;
bool connected = false;

Menu clientMenu("Menu de opciones del Cliente");

void srvConnect() {
  if(connected) {
    cout << endl << "Ya hay una conexion activa" << endl;
    clientMenu.display();
    return;
  }

  const int PORT = 5340;
  const int MAX_DATA_SIZE = 100; /* Max. number of bytes for recv */
  int sfd, numBytesRead;
  char buf[MAX_DATA_SIZE]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */
  const char* IP = "192.168.1.108";

  /* Create socket */
  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cout << "socket error" << endl;
    exit(-1);
  }

  gfd = sfd;

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if((inet_aton(IP, &server.sin_addr)) == 0) {
    cout << "invalid IP" << endl;
    exit(-1);
  }

  bzero(&(server.sin_zero), 8);

  /* Connect to server */
  short triesLeft = 3;

  while(!connected && triesLeft > 0) {
    if(connect(sfd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) {
      cout << endl << "Error de conexion." << endl << "Reintentando conexion..." << endl;
      triesLeft--;
      /* 5s delay for retry */
      usleep(5000000);
    } else {
      connected = true;
      cout << "Se establecio una conexion con: " << IP << endl;
    }

    if(triesLeft == 0) {
      cout << endl << "No se pudo establecer una conexion con el servidor: " << IP 
           << endl << "Por favor intente nuevamente mas tarde." << endl;
    }
  }
  
  clientMenu.display();

  /*
  while(connected) {
    if((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
      cout << "recv error" << endl;
      exit(-1);
    }

    if(numBytesRead) {
      buf[numBytesRead] = '\0';
      cout << "Mensaje del servidor: " << buf << endl;
    }
  }*/
  //close(sfd);
}

void srvDisconnect() {
  if(connected) {
    close(gfd);  
    connected = false;
    cout << endl << "Se cerro la conexion con el servidor." << endl;
  } else {
    cout << endl << "No hay una conexion activa" << endl;
  }
  clientMenu.display();
}

void cycle() {}

void exitPgm() {}

int main(int argc, char* argv[]) {
  
  const char* fileName = argv[1] ? argv[1] : "default.xml";

  clientMenu.addOption("Conectar", srvConnect);
  clientMenu.addOption("Desconectar", srvDisconnect);
  clientMenu.addOption("Ciclar", cycle);
  clientMenu.addOption("Salir", exitPgm);

  clientMenu.display();

  return 0;
}