// server.cpp
#include <sys/socket.h> /* socket, bind, listen, accept, connect, close */
#include <sys/types.h> /* socket, bind, listen, accept, connect, close */
#include <netdb.h> /* addrinfo */
#include <netinet/in.h>
#include <unistd.h> /* close() */
#include <arpa/inet.h> /* inet_ntop() */
#include <cstdint> /* uint32_t */
#include <csignal> /* signal(,) */
#include <cstdlib> /* exit() */
#include <cstring> /* memset(,,) */
#include <iostream>
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"
#include <mutex>
#include <thread>
#include <queue>
#include "../xml/parser/XMLParser.h"
#include "../xml/conf/ServerConf.h"

using namespace std;

Menu serverMenu("Menu de opciones del Servidor");
queue<const char*>* msgQueue = new queue<const char*>;
mutex theMutex;
ServerConf* sc;

int clientCount = 0;

int gfd = 0;
bool listening = false;
bool serverConnected = false;

void closeConnection() {
  close(gfd);
  listening = false;
  cout << endl << warning("Desconectando servidor...") << endl;
}

void closeClient(int cfd) {
  close(cfd);
  theMutex.lock();
  clientCount--;
  cout << "cantidad " << clientCount << endl;
  theMutex.unlock();
}

// get sockaddr, IPv4 
void* get_in_addr(struct sockaddr* sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*) sa)->sin_addr);
  }
}

void recieveClientData(int cfd, struct sockaddr_storage client_addr, bool allowConnections) {
  int numBytesRead;
  char clientIP[INET_ADDRSTRLEN]; // connected client IP
  const int MAX_DATA_SIZE = 100;
  char buf[MAX_DATA_SIZE]; // data buffer

  // get connected host IP in presentation format
  inet_ntop(client_addr.ss_family,
      get_in_addr((struct sockaddr*) (&client_addr)), clientIP,
      sizeof clientIP);

  cout << endl << notice("Se inicio una conexion con el host: ") << clientIP
       << endl;

  if (allowConnections) {
    if (send(cfd, "Aceptado", 12, 0) == -1) {
      cout << "send error" << endl;
    }
    bool receiving = true;
    while (receiving) {
      if ((numBytesRead = recv(cfd, buf, MAX_DATA_SIZE, 0)) == -1) {
	cout << "recv error" << endl;
      }
      if (numBytesRead) {
	buf[numBytesRead] = '\0';
	cout << "************************" << endl;
	theMutex.lock();
	msgQueue->push(buf);
	cout << endl << "Pongo mensaje del cliente: " << buf << endl;
	theMutex.unlock();
	cout << "************************" << endl;
      } else {
	receiving = false;
	cout << endl << warning("El cliente ") << clientIP
	     << warning(" se desconecto") << endl;
	closeClient(cfd);
      }
    }
  } else {
    cout << endl << warning("El cliente ") << clientIP
	 << warning(" se rechazo") << endl;
    closeClient(cfd);
  }
}

void serverListening(int sfd, int cfd, struct sockaddr_storage client_addr, socklen_t sinSize) {
  // accept connections
  while (listening) {
    sinSize = sizeof client_addr;
	if ((cfd = accept(sfd, (struct sockaddr*) (&client_addr), &sinSize)) == -1) {
	  cout << "accept error" << endl;
	  exit(-1);
	}
	clientCount++;
	bool allowConnections = (clientCount <= sc->getMaxClients());
	thread process(recieveClientData, cfd, client_addr, allowConnections);
	process.detach();
  }
}

void serverInit() {
  int sfd, cfd;
  struct sockaddr_storage client_addr; // client address information
  socklen_t sinSize;
  if (serverConnected) {
	  cout << endl << warning("El servidor ya se encuentra conectado") << endl;
	  serverMenu.display();
  } else {
  //  const char* PORT = "5340";
  const int BACKLOG = 5;
   // socket and client file descriptors
  struct addrinfo hints, *servinfo, *p; // configuration structs
  int rv; //, numBytesRead;

  // init hints struct with 0
  memset(&hints, 0, sizeof hints);

  // set hints struct values
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use host IP

  // fill configuration structs
  if ((rv = getaddrinfo(NULL, to_string(sc->getPort()).c_str(), &hints, &servinfo)) != 0) {
    cout << "getaddrinfo error" << gai_strerror(rv) << endl;
    exit(-1);
  }

  int yes = 1;
  // loop through results and bind to one of them
  for (p = servinfo; p != NULL; p = p->ai_next) {
    // try to create TCP socket
    if ((sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
	== -1) {
      cout << "socket error" << endl;
      continue; // try next one
    }

    // allow port reuse to avoid bind error
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
	== -1) {
      cout << "setsockopt error" << endl;
      exit(-1);
    }

    // bind socket
    if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sfd);
      cout << "bind error" << endl;
      continue; // try next one
    }

    break; // socket created and binded
  }
  freeaddrinfo(servinfo); // free memory

  if (p == NULL) {
    cout << "server failed to bind" << endl;
    exit(-1);
  }

  // listen for connections
  if (listen(sfd, BACKLOG) == -1) {
    cout << "listen error" << endl;
    exit(-1);
  }

  listening = true;
  cout << endl << notice("Se ha iniciado el servidor") << endl
    << "Esperando conexiones..." << endl;
  serverConnected = true;

  // accept connections
  std::thread t2 (serverListening, sfd, cfd, client_addr, sinSize);
  t2.detach();
  }

}

void exitPgm() {
  cout << endl << warning("Cerrando el servidor...") << endl;
  exit(0);
}

void threadProcesador() {
  while (true) {
    if (!msgQueue->empty()) {
      theMutex.lock();
      cout << "------------------------------------" << endl;
      cout << "Saco Msj de la cola" << endl;
      cout << msgQueue->front() << endl;
      msgQueue->pop();
      cout << "------------------------------------" << endl;
      theMutex.unlock();
    }
  }
}

int main(int argc, char* argv[]) {
  std::thread t1(threadProcesador);

  const char* fileName = argv[1] ? argv[1] : "default-sc.xml";
  sc = XMLParser::parseServerConf(fileName);

  serverMenu.addOption("Iniciar servidor", serverInit);
  serverMenu.addOption("Salir", exitPgm);

  serverMenu.display();
  t1.join();
  return 0;
}
