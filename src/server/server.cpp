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
#include "../logger/Logger.h"
#include "../xml/parser/XMLParser.h"
#include "../xml/conf/ServerConf.h"

using namespace std;

Logger* logger = Logger::instance();

const int MAX_CHAR_LENGTH = 20;

Menu serverMenu("Menu de opciones del Servidor");
queue<map<int,char*>*>* msgQueue = new queue<map<int,char*>*>;

mutex theMutex;
ServerConf* sc;

int clientCount = 0;

map<int,char*>* clientFD;

int gfd = 0;
bool listening = false;
bool serverConnected = false;

void closeConnection() {
  close(gfd);
  listening = false;
  cout << endl << warning("Desconectando servidor...") << endl;
  logger->warn("Desconectando servidor...");
}

void closeClient(int cfd) {
  close(cfd);
  theMutex.lock();
  clientCount--;
  cout << "cantidad " << clientCount << endl;
  logger->info("Cantidad de Clientes Conectados: " + to_string(clientCount));
  theMutex.unlock();
}

// get sockaddr, IPv4 
void* get_in_addr(struct sockaddr* sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*) sa)->sin_addr);
  }
}

void recieveClientData(int cfd, struct sockaddr_storage client_addr,
    bool allowConnections) {
  int numBytesRead;
  char clientIP[INET_ADDRSTRLEN]; // connected client IP
  const int MAX_DATA_SIZE = 10000;
  char buf[MAX_DATA_SIZE]; // data buffer

  // get connected host IP in presentation format
  inet_ntop(client_addr.ss_family,
      get_in_addr((struct sockaddr*) (&client_addr)), clientIP,
      sizeof clientIP);

  cout << endl << notice("Se inicio una conexion con el host: ") << clientIP
    << endl;
  logger->info("Se inicio una conexion con el host: " + string(clientIP));

  if (allowConnections) {
    if (send(cfd, "Aceptado", 12, 0) == -1) {
      logger->error("Error al enviar que se acepto la conexion");
    }
    bool receiving = true;
    while (receiving) {
      if ((numBytesRead = recv(cfd, buf, MAX_DATA_SIZE, 0)) == -1) {
	logger->error("Falla al recibir msj del cliente");
      }
      if (numBytesRead) {
	buf[numBytesRead] = '\0';
	theMutex.lock();

	clientFD = new map<int,char*>();
	clientFD->insert(pair<int,char*>(cfd,buf));

	msgQueue->push(clientFD);

	cout << endl << "Pongo mensaje del cliente: " << buf << endl;
	theMutex.unlock();

      } else {
	receiving = false;
	cout << endl << warning("El cliente ") << clientIP
	  << warning(" se desconecto") << endl;
	logger->warn("El Cliente " + string(clientIP) + " se desconecto");

	closeClient(cfd);
      }
    }
  } else {
    cout << endl << warning("El cliente ") << clientIP << warning(" se rechazo")
      << endl;
    logger->warn("El cliente " + string(clientIP) + " se rechazo");
    closeClient(cfd);
  }
}

void serverListening(int sfd, int cfd, struct sockaddr_storage client_addr, socklen_t sinSize) {
  // accept connections
  while (listening) {
    sinSize = sizeof client_addr;
    if ((cfd = accept(sfd, (struct sockaddr*) (&client_addr), &sinSize))
	== -1) {
      logger->error("Error al aceptar Cliente");
      exit(-1);
    }
    clientCount++;
    bool allowConnections = (clientCount <= sc->getMaxClients());
    thread process(recieveClientData, cfd, client_addr, allowConnections);
    process.detach();
  }
}

void serverInit() {
  if (serverConnected) {
    cout << endl << warning("El servidor ya se encuentra conectado") << endl;
    logger->warn("El servidor ya se encuentra conectado");
    serverMenu.display();
  } else {
    int sfd, cfd; // socket and client file descriptors
    struct sockaddr_storage client_addr; // client address information
    socklen_t sinSize;
    const int BACKLOG = 5;
    struct addrinfo hints, *servinfo, *p; // configuration structs
    int rv; 

    // init hints struct with 0
    memset(&hints, 0, sizeof(hints));

    // set hints struct values
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use host IP

    // fill configuration structs
    if ((rv = getaddrinfo(NULL, to_string(sc->getPort()).c_str(), &hints, &servinfo)) != 0) {
      logger->error(
	  "Error al obtener la direccion, " + string(gai_strerror(rv)));
      exit(-1);
    }

    int yes = 1;
    // loop through results and bind to one of them
    for (p = servinfo; p != NULL; p = p->ai_next) {
      // try to create TCP socket
      if ((sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	logger->error("Error en el socket");
	continue; // try next one
      }

      // allow port reuse to avoid bind error
      if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	logger->error("Falla en la configuracion del socket");
	exit(-1);
      }

      // bind socket
      if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1) {
	close(sfd);
	logger->error("bind error");
	continue; // try next one
      }

      break; // socket created and binded
    }
    freeaddrinfo(servinfo); // free memory

    if (p == NULL) {
      logger->error("Falla en el bind del servidor");
      exit(-1);
    }

    // listen for connections
    if (listen(sfd, BACKLOG) == -1) {
      logger->error("listen error");
      exit(-1);
    }

    listening = true;
    cout << endl << notice("Se ha iniciado el servidor") << endl
      << "Esperando conexiones..." << endl;
    logger->info("se ha iniciado el servidor. Esperando conexiones...");
    serverConnected = true;

    // accept connections
    thread t2(serverListening, sfd, cfd, client_addr, sinSize);
    t2.detach();
  }

}

void exitPgm() {
  cout << endl << warning("Cerrando el servidor...") << endl;
  logger->warn("Cerrando el servidor...");
  delete msgQueue;
  exit(0);
}

void sendingData(int cfd, string data, int dataLength){
  bool notSent = true;
  //TODO: falta agregar de que no loopee si llega a estar desconectado el cliente
  while (notSent){
    if (send(cfd, data.c_str(), dataLength, 0) == -1) {
      cout << "send error" << endl;
    }else{
      notSent = false;
    }
  }
}

void threadProcesador() {
  while (true) {
    if (!msgQueue->empty()) {
      theMutex.lock();
      cout << "Saco Msj de la cola" << endl;
      map<int,char*>* data = msgQueue->front();
      msgQueue->pop();

      map<int,char*>::iterator it = data->begin();
      cout << "IP cliente: " << it->first << " --  Mensaje: " << it->second << endl;

      logger->info("Msj de cliente: " + string(it->second));

      thread tSending(sendingData, it->first, it->second , MAX_CHAR_LENGTH);
      tSending.detach();

      delete clientFD;

      theMutex.unlock();
    }
  }
}

int main(int argc, char* argv[]) {
  thread t1(threadProcesador);
  const char* fileName = argv[1] ? argv[1] : "default-sc.xml";
  sc = XMLParser::parseServerConf(fileName);

  serverMenu.addOption("Iniciar servidor", serverInit);
  serverMenu.addOption("Salir", exitPgm);

  serverMenu.display();
  t1.join();
  return 0;
}
