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
#include <regex>

using namespace std;

const int MAX_CHAR_LENGTH = 10;
Menu serverMenu("Menu de opciones del Servidor");
//queue<const char*>* msgQueue = new queue<const char*>;
queue<map<int,char*>*>* msgQueue = new queue<map<int,char*>*>;

mutex theMutex;

int maxClientCount = 2;
int clientCount = 0;

map<int,char*>* clientFD;

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

void recieveClientData(int cfd, struct sockaddr_storage client_addr,
		bool allowConnections) {
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

				clientFD = new map<int, char*>();
				clientFD->insert(pair<int, char*>(cfd, buf));

				msgQueue->push(clientFD);

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
		cout << endl << warning("El cliente ") << clientIP << warning(" se rechazo")
				<< endl;
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
	bool allowConnections = (clientCount <= maxClientCount);
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
  const char* PORT = "5340";
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
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
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

string serverProcess (string tipo, string valor){
	const int MAX_INT = 2147483647;
	bool respuesta = false;
	string mensaje;
	regex r;
	const char* expr;

	if(tipo == "INT"){
		//expr = "^-?(2?1?[0-4]?|2?0?[0-9]?|[0-1]?[0-9]?[0-9]?)([0-9]){1,7}$";//menor que +-2148000000
		expr = "^-?[0-9]+$";
		r = regex(expr);
		if ((regex_match(valor, r)) && (atoi(valor.c_str()) >= -MAX_INT) && (atoi(valor.c_str()) <= MAX_INT)) //ese casteo de char* a int no se si se puede
			respuesta = true;

	} else {

		if (tipo == "DOUBLE"){
			expr = "^-?([0-2]e-?[0-9]{1,3}|[0-2][//.][0-9]{0,2}e-?[0-9]{1,3}|[//.0-9]+)$";
			r = regex(expr);
			if (regex_match(valor, r)) respuesta = true;

		} else {

			if (tipo == "STRING"){
			  expr = "^.+$";
			  r = regex(expr);
			  if (regex_match(valor, r)) respuesta = true;

			} else {

				if (tipo == "CHAR"){
					 expr = "^.$";
					 r = regex(expr);
					 if (regex_match(valor, r)) respuesta = true;
				}
			}
		}
	}

	if (respuesta) {
		mensaje = "Mensaje Correcto";
	} else {
		mensaje = "Mensaje Incorrecto";
	}

	return mensaje;
}

void threadProcesador() {
	string respuesta;
  while (true) {
    if (!msgQueue->empty()) {
      theMutex.lock();
      cout << endl << "------------------------------------" << endl;
      cout << "Saco Msj de la cola" << endl;
      clientFD = msgQueue->front();
      msgQueue->pop();
      cout << "------------------------------------" << endl;

      map<int,char*>::iterator it = clientFD->begin();
      cout << it->first << it->second << endl;

      //proceso el dato recibido
      string valor; //hardcodeado
      string tipo; //hardcodeado
      respuesta = serverProcess (tipo, valor); //hardcodeado en it->second deberia estar el tipo

			//thread tSending(sendingData, it->first, it->second , MAX_CHAR_LENGTH);
      thread tSending(sendingData, it->first, respuesta , MAX_CHAR_LENGTH);
			tSending.detach();

		  delete clientFD;

			theMutex.unlock();
    }
  }
}

int main() {
  std::thread t1(threadProcesador);

  serverMenu.addOption("Iniciar servidor", serverInit);
  serverMenu.addOption("Salir", exitPgm);

  serverMenu.display();
  t1.join();
  return 0;
}
