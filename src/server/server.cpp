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
#include <regex>
#define DEBUG 1
#include "../libs/debug/dg_msg.h"
#include "../utils/K.h"

using namespace std;
bool serverProcessing;
Logger* logger = Logger::instance();
const int MAX_CHAR_LENGTH = 20;
Menu serverMenu("Menu de opciones del Servidor");
queue<map<int, Mensaje*>*>* msgQueue = new queue<map<int, Mensaje*>*>;

mutex theMutex;
ServerConf* sc;

int clientCount = 0;

int gfd = 0;
bool listening = false;
bool serverConnected = false;
const int MAX_UNREACHABLE_TIME = 5;

void closeConnection() {
  delete msgQueue;
  close(gfd);
  listening = false;
  serverConnected = false;
  serverProcessing = false;
  logger->warn(SERVER_DISCONNECT);
  DEBUG_WARN(SERVER_DISCONNECT);
}

void exitPgm() {
  if(serverConnected)
    closeConnection();
  logger->warn(SERVER_CLOSE);
  //DEBUG_WARN(SERVER_CLOSE);

  exit(0);
}

void checkAliveSend(int sfd) {
  char buf[1] = { '1' };

  while(true) {
    if(!serverConnected) return;
    // 4s timed send
    usleep(4000000);
    send(sfd, &buf, 1, 0);
  }
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
   
    timeval timeout;
    timeout.tv_sec = MAX_UNREACHABLE_TIME;
    timeout.tv_usec = 0;
    bool receiving = true;
    while (receiving) {
      // seteo el timeout de recepcion de mensajes
      if(setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout)) < 0) {
	cout << "Error sockopt" << endl;
	exit(1);
      }

      if ((numBytesRead = recv(cfd, msgToRecv, sizeof(Mensaje), 0)) == -1) {
	close(cfd);
	logger->warn(CONNECTION_TIMEOUT);
	DEBUG_WARN(CONNECTION_TIMEOUT);
	exitPgm();
      }
      
      if (numBytesRead > 0) {
	if(numBytesRead != 1) {
	  theMutex.lock();
	  cout << endl << "ID del mensaje recibido: " << notice(msgToRecv->id) << endl;
	  cout << "Tipo del mensaje recibido: " << notice(msgToRecv->tipo) << endl;
	  cout << "Valor del mensaje recibido: " << notice(msgToRecv->valor) << endl;

	  map<int,Mensaje*>* clientMsgFD = new map<int,Mensaje*>();
	  clientMsgFD->insert(pair<int,Mensaje*>(cfd, msgToRecv));
	  msgQueue->push(clientMsgFD);
	  theMutex.unlock();
	}
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

    thread tCheckAliveSend(checkAliveSend, cfd);
    tCheckAliveSend.detach();

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

void sendingData(int cfd, Mensaje* data, int dataLength){
  //bool notSent = true;
  //TODO: falta agregar de que no loopee si llega a estar desconectado el cliente
  //while (notSent){
  if (send(cfd, data, dataLength, 0) == -1) {
    logger->warn(SEND_FAIL);
    DEBUG_WARN(SEND_FAIL);
    //}else{
    //notSent = false;
    //}
}
}

bool serverProcess (string tipo, string valor){
  const int MAX_INT = 2147483647;
  bool respuesta = false;

  regex r;
  const char* expr;

  if(tipo == K::typeInt){
    //expr = "^-?(2?1?[0-4]?|2?0?[0-9]?|[0-1]?[0-9]?[0-9]?)([0-9]){1,7}$";//menor que +-2148000000
    expr = "^-?[0-9]+$";
    r = regex(expr);
    if ((regex_match(valor, r)) && (atoi(valor.c_str()) >= -MAX_INT) && (atoi(valor.c_str()) <= MAX_INT)) //ese casteo de char* a int no se si se puede
      respuesta = true;

  } else {

    if (tipo == K::typeDouble){
      expr = "^-?([0-2]e-?[0-9]{1,3}|[0-2][//.][0-9]{0,2}e-?[0-9]{1,3}|[0-9]+[//.][0-9]+)$";
      r = regex(expr);
      if (regex_match(valor, r)) respuesta = true;

    } else {

      if (tipo == K::typeString){
	expr = "^.+$";
	r = regex(expr);
	if (regex_match(valor, r)) respuesta = true;

      } else {

	if (tipo == K::typeChar){
	  expr = "^.$";
	  r = regex(expr);
	  if (regex_match(valor, r)) respuesta = true;
	}
      }
    }
  }
  return respuesta;
}

void threadProcesador() {
  bool esCorrecto;
  Mensaje* respuesta = new Mensaje;

  while (serverProcessing) {
    if (!msgQueue->empty()) {
      theMutex.lock();
      cout << "Saco Msj de la cola" << endl;
      map<int,Mensaje*>* data = msgQueue->front();
      msgQueue->pop();

      map<int,Mensaje*>::iterator it = data->begin();
      cout << "FD cliente: " << it->first << " --  Mensaje: " << (it->second)->valor << endl;

      logger->info("Msj de cliente: " + string(((it->second)->valor)));

      esCorrecto = serverProcess(string((it->second)->tipo), string(((it->second)->valor)));
      if (esCorrecto) {
	strcpy(respuesta->valor, "Mensaje Correcto");
      } else {
	strcpy(respuesta->valor, "Mensaje Incorrecto");
      }
      thread tSending(sendingData, it->first, respuesta , sizeof(Mensaje));
      tSending.detach();

      delete data;

      theMutex.unlock();
    }
  }
  cout<<"Corto processor"<<endl;
  delete respuesta;
}

int main(int argc, char* argv[]) {
  serverProcessing = true;
  thread processor(threadProcesador);
  const char* fileName = argv[1] ? argv[1] : "default-sc.xml";
  sc = XMLParser::parseServerConf(fileName);
  serverMenu.addOption("Iniciar servidor", serverInit);
  // TODO: add disconnect option using srvDisconnect
  serverMenu.addOption("Salir", exitPgm);

  serverMenu.display();
  processor.join();
  return 0;
}
