#include "../libs/socket/sock_dep.h" /* socket dependencies */
#include "../libs/menu/Menu.h"
#include "../libs/palette/palette.h"
#include "../logger/Logger.h"
#include "../xml/parser/XMLParser.h"
#include "../xml/conf/ServerConf.h"
#include "../libs/mensaje/mensaje.h"
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#define DEBUG 1
#include "../libs/debug/dg_msg.h"

using namespace std;

Logger* logger = Logger::instance();
const int MAX_CHAR_LENGTH = 20;
Menu serverMenu("Menu de opciones del Servidor");
queue<map<int, Mensaje*>*>* msgQueue = new queue<map<int, Mensaje*>*>;

mutex theMutex;
ServerConf* sc;

int clientCount = 0;
map<int,Mensaje*>* clientFD;

int gfd = 0;
bool listening = false;
bool serverConnected = false;

void closeConnection() {
  delete msgQueue;
  close(gfd);
  listening = false;
  serverConnected = false;
  logger->warn(SERVER_DISCONNECT);
  DEBUG_WARN(SERVER_DISCONNECT);
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
  Mensaje* msgToRecv = new Mensaje;

  // get connected host IP in presentation format
  inet_ntop(client_addr.ss_family,
      get_in_addr((struct sockaddr*) (&client_addr)), clientIP,
      sizeof clientIP);

  if (allowConnections) {

    cout << endl << notice("Se inicio una conexion con el host: ") << clientIP
      << endl;
    logger->info("Se inicio una conexion con el host: " + string(clientIP));


    if (send(cfd, "Aceptado", 12, 0) == -1) {
      logger->error("Error al enviar que se acepto la conexion");
    }
    bool receiving = true;
    while (receiving) {
      if ((numBytesRead = recv(cfd, msgToRecv, sizeof(Mensaje), 0)) == -1) {
	logger->error("Falla al recibir msj del cliente");
      }
      if (numBytesRead) {
	cout << endl << "ID del mensaje recibido: " << notice(msgToRecv->id) << endl;
	cout << "Tipo del mensaje recibido: " << notice(msgToRecv->tipo) << endl;
	cout << "Valor del mensaje recibido: " << notice(msgToRecv->valor) << endl;

	theMutex.lock();

	clientFD = new map<int,Mensaje*>();
	clientFD->insert(pair<int,Mensaje*>(cfd, msgToRecv));

	msgQueue->push(clientFD);

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
    usleep(1000000);
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
    logger->warn(CONNECTION_ACTIVE);
    DEBUG_WARN(CONNECTION_ACTIVE);
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
      logger->error("Error al obtener la direccion, " + string(gai_strerror(rv)));
      exit(-1);
    }

    int yes = 1;
    // loop through results and bind to one of them
    for (p = servinfo; p != NULL; p = p->ai_next) {
      // try to create TCP socket
      if ((sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
	logger->error(SOCKET_ERROR);
	continue; // try next one
      }

      // allow port reuse to avoid bind error
      if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
	logger->error(SOCKET_REUSE_ERROR);
	exit(-1);
      }

      // bind socket
      if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1) {
	close(sfd);
	logger->error(BIND_ERROR);
	continue; // try next one
      }

      break; // socket created and binded
    }
    freeaddrinfo(servinfo); // free memory

    if (p == NULL) {
      logger->error(BIND_CERROR);
      exit(-1);
    }

    // listen for connections
    if (listen(sfd, BACKLOG) == -1) {
      logger->error(LISTEN_ERROR);
      exit(-1);
    }

    listening = true;
    logger->info(SERVER_START);
    DEBUG_NOTICE(SERVER_START);
    serverConnected = true;

    // accept connections
    thread t2(serverListening, sfd, cfd, client_addr, sinSize);
    t2.detach();
  }

}

void srvDisconnect() {
  if(serverConnected) {
    closeConnection();
  } else {
    logger->warn(CONNECTION_NOT_ACTIVE); 
    DEBUG_WARN(CONNECTION_NOT_ACTIVE);
  }
}

void exitPgm() {
  if(serverConnected)
    closeConnection();
  logger->warn(SERVER_CLOSE);
  //DEBUG_WARN(SERVER_CLOSE);

  exit(0);
}

void sendingData(int cfd, Mensaje* data, int dataLength){
  bool notSent = true;
  //TODO: falta agregar de que no loopee si llega a estar desconectado el cliente
  while (notSent){
    if (send(cfd, data, dataLength, 0) == -1) {
      logger->warn(SEND_FAIL);
      DEBUG_WARN(SEND_FAIL);
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
      map<int,Mensaje*>* data = msgQueue->front();
      msgQueue->pop();

      map<int,Mensaje*>::iterator it = data->begin();
      cout << "FD cliente: " << it->first << " --  Mensaje: " << (it->second)->valor << endl;

      logger->info("Msj de cliente: " + string(((it->second)->valor)));

      thread tSending(sendingData, it->first, it->second , sizeof(Mensaje));
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
  // TODO: add disconnect option using srvDisconnect
  serverMenu.addOption("Salir", exitPgm);

  serverMenu.display();
  t1.join();
  return 0;
}
