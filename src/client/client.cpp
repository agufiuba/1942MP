// client.cpp
#include <sys/socket.h>
#include <netinet/in.h> /* IPv4 & IPv6 presentation strings max.length constants (INET6?_ADDRSTRLEN) */
#include <arpa/inet.h> /* inet_aton(,) */
#include <netdb.h> /* hostent */
#include <unistd.h> /* close() */
#include <cstdlib> /* exit() */
#include <cstring> /* memset(), bzero() */
#include <iostream>

using namespace std;

int main() {
  
  const int PORT = 5340;
  const int MAX_DATA_SIZE = 100; /* Max. number of bytes for recv */
  int sfd, numBytesRead;
  char buf[MAX_DATA_SIZE]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */
  const char* IP = "192.168.1.101";

  /* Create socket */
  if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    cout << "socket error" << endl;
    exit(-1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  if((inet_aton(IP, &server.sin_addr)) == 0) {
    cout << "invalid IP" << endl;
    exit(-1);
  }

  bzero(&(server.sin_zero), 8);

  /* Connect to server */
  bool connected = false;
  short triesLeft = 3;

  while(!connected) {
    if(triesLeft == 0) {
      cout << "No se pudo establecer una conexion con el servidor: " << IP 
           << endl << "Por favor intente nuevamente mas tarde." << endl;
      exit(-1);
    }

    if(connect(sfd, (struct sockaddr*) &server, sizeof(struct sockaddr)) == -1) {
      cout << "Error de conexion." << endl << "Reintentando conexion..." << endl << endl;
      triesLeft--;
      /* 5s delay for retry */
      usleep(5000000);
    } else {
      connected = true;
      cout << "Se establecio una conexion con: " << IP << endl;
    }
  }

  if((numBytesRead = recv(sfd, buf, MAX_DATA_SIZE, 0)) == -1) {
    cout << "recv error" << endl;
    exit(-1);
  }

  buf[numBytesRead] = '\0';
  cout << "Mensaje del servidor: " << buf << endl;
  close(sfd);

  return 0;
}
