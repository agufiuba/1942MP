#include "Client.h"
#include "../socket/sock_dep.h" /* socket dependencies */
#include "../../xml/parser/XMLParser.h"
#include "../palette/palette.h"
#define DEBUG 1
#include "../debug/dg_msg.h"
#include <string>
#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

Client::Client( const char* configFileName ) {
  this->socketFD = 0;
  this->connected = false;
  this->logger = Logger::instance();

//  this->config = XMLParser::parseClientConf( configFileName );
}

Client::Client( string ip, string puerto ) {
  this->socketFD = 0;
  this->connected = false;
  this->logger = Logger::instance();
  this->ip = ip;
  this->puerto = puerto;

//  this->config = XMLParser::parseClientConf( configFileName );
}

Client::~Client() {}

bool Client::connectToServer() {
  mutex theMutex;

  if( this->connected ) {
    this->logger->warn( CONNECTION_ACTIVE );
    theMutex.lock();
    DEBUG_WARN( CONNECTION_ACTIVE );
    theMutex.unlock();
    return false;
  }

  const int MAX_DATA_SIZE = 10000; /* Max. number of bytes for recv */
  int numBytesRead; /* socketFD, bytes read count */
  char buf[ MAX_DATA_SIZE ]; /* Received text buffer  */
  struct sockaddr_in server; /* Server address info */

  /* Create socket */
  if ( ( this->socketFD = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 ) {
    this->logger->error( SOCKET_ERROR );
    //exit( -1 );
    return false;
  }

  server.sin_family = AF_INET;

  server.sin_port = htons(stoi(this->puerto));
  if ((inet_aton(this->ip.c_str(), &server.sin_addr)) == 0) {
    this->logger->error( "IP invalido" );
    return false;
    //exit( -1 );
  }

  bzero( &( server.sin_zero ), 8 );

  /* Connect to server */
  short triesLeft = 3;

	while (!(this->connected) && triesLeft) {
		if (connect(this->socketFD, (struct sockaddr*) &server,
				sizeof(struct sockaddr)) == -1) {
			triesLeft--;
			/* 5s delay for retry */
			if (triesLeft) {
				this->logger->error( CONNECTION_RETRY);
				theMutex.lock();
				DEBUG_WARN(CONNECTION_RETRY);
				theMutex.unlock();
				usleep(1000000);
			} else {
				this->logger->warn(CONNECTION_ERROR(this->ip));
				theMutex.lock();
				DEBUG_PRINT(CONNECTION_ERROR( this->ip ));
				theMutex.unlock();
				return false;
			}
		} else {
			this->connected = true;
		}
	}

  timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 5000;

  // seteo el timeout de recepcion de mensajes
  if( setsockopt( this->socketFD, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
	sizeof( timeout ) ) < 0 ) {
    cout << "Error sockopt" << endl;
    exit( 1 );
  }

  // Get server welcome message
  if( ( numBytesRead = recv( this->socketFD, buf, MAX_DATA_SIZE, 0 ) ) == -1 ) {
    this->logger->warn( CONNECTION_REJECTED );
    theMutex.lock();
    DEBUG_WARN( CONNECTION_REJECTED );
    theMutex.unlock();
    this->connected = false;
    close( this->socketFD );
  }
  if( numBytesRead > 0 ) {
    this->logger->info( CONNECTION_SUCCESS( this->ip ) );
    theMutex.lock();
    DEBUG_PRINT( CONNECTION_SUCCESS( this->ip ) );
    buf[ numBytesRead ] = '\0';
    string recvMsg = string( buf );
    this->logger->info( SERVER_MSG( recvMsg ) );
    DEBUG_PRINT( SERVER_MSG( recvMsg ) );
    theMutex.unlock();
    return true;
  }
  if( numBytesRead == 0 ) {
    this->logger->warn( CONNECTION_LOST );
    theMutex.lock();
    DEBUG_WARN( CONNECTION_LOST );
    theMutex.unlock();
    this->connected = false;
    close( this->socketFD );
  }

  // Create thread for receiving data from server
  if( this->connected ) {
    thread tCheckAliveSend( &Client::checkAliveSend, this );
    tCheckAliveSend.detach();

    thread tReceiving( &Client::receiving, this,  MAX_DATA_SIZE, this->ip.c_str() );
    tReceiving.detach();
  }
}

void Client::checkAliveSend() {
  char buf[1] = { '1' };

  while( true ) {
    if( !( this->connected ) ) return;
    // 4s timed send
    usleep( 4000000 );
    send( this->socketFD, &buf, 1, 0 );
  }
}

void Client::receiving( const int MAX_DATA_SIZE, const char *IP ){
  int numBytesRead;
  timeval timeout;
  timeout.tv_sec = this->MAX_UNREACHABLE_TIME;
  timeout.tv_usec = 0;
  Mensaje* buf = new Mensaje;

  while ( this->connected ) {
    // seteo el timeout de recepcion de mensajes
    if( setsockopt( this->socketFD, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof(timeout) ) < 0 ) {
      cout << "Error sockopt" << endl;
      exit( 1 );
    }

    if( ( numBytesRead = recv( this->socketFD, buf, MAX_DATA_SIZE, 0 ) ) == -1 ) {
      close( this->socketFD );
      this->connected = false;
      this->logger->warn( CONNECTION_TIMEOUT );
      DEBUG_WARN( CONNECTION_TIMEOUT );
      return;
    }

    if( numBytesRead > 0 ) {
      if( numBytesRead != 1 ) {
	string recvMsg = string( buf->valor );
	this->logger->info( SERVER_MSG( recvMsg ) );
	DEBUG_PRINT( SERVER_MSG( recvMsg ) );
	this->received = true;
      }
    }
    if( numBytesRead == 0 ){
      this->logger->warn( CONNECTION_LOST );
      DEBUG_WARN( CONNECTION_LOST );
      this->connected = false;
      close( this->socketFD );
    }
  }
}

bool Client::sendData( Evento* e ) {
  this->received = false;
  if( send( this->socketFD, e, sizeof(Evento), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    //    theMutex.lock();
    DEBUG_WARN( SEND_FAIL );
    //    theMutex.unlock();
    return false;
  }

  return true;
}

//bool Client::sendMsg( string id ) {
//  if( !( this->connected ) ) {
//    this->logger->warn( SEND_CERROR );
//    //  theMutex.lock();
//    DEBUG_WARN( SEND_CERROR );
//    // theMutex.unlock();
//    return false;
//  }
//
//  Mensaje* msgToSend;
//
//  for( int i = 0; i < this->config->getMessages().size(); i++ ) {
//    string msgID = this->config->getMessages()[i]->id;
//    if( msgID == id ) {
//      msgToSend = this->config->getMessages()[i];
//      break;
//    }
//  }
//
//  int dataLength = sizeof( Mensaje );
//  this->logger->info( SENT_DATA( msgToSend->valor ) );
//  // theMutex.lock();
//  DEBUG_PRINT( SENT_DATA( msgToSend->valor ) );
//  // theMutex.unlock();
//  return sendData( msgToSend, dataLength );
//}

//void Client::sendCycle() {
//  if( !( this->connected ) ) {
//    this->logger->warn( CONNECTION_NOT_ACTIVE );
//    DEBUG_WARN( CONNECTION_NOT_ACTIVE );
//    return;
//  }
//
//  double timeout = 0;
//  cout << "Ingrese duracion (en milisegundos): ";
//  cin >> timeout;
//
//  while( timeout <= 0 ) {
//    cout << endl << "Error - Debe ingresar un numero mayor a cero" << endl;
//    cout << "Ingrese nuevamente durancion (en milisegundos): ";
//    cin >> timeout;
//  }
//  timeout = timeout * 1000;
//
//  this->logger->info( "Se corre ciclar en " + to_string(timeout) + " milisegundos." );
//  double diferencia = 0;
//  int i = 0;
//  this->received = true;
//  clock_t start = clock();
//  while( diferencia <= timeout && this->connected ) {
//    if( this->received ){
//      if( i >= this->config->getMessages().size() )
//	i = 0;
//      //cout << endl << "En el i: " << i;
//
//      if( !( sendData(getEventos()[i]) ) )
//	return;
//
//      i++;
//    }
//    diferencia = clock() - start;
//  }
//  usleep( 10000 ); // solo para que el ultimo se muestre antes de hacer display del menu
//}

void Client::disconnectFromServer() {
  mutex theMutex;
  if( this->connected ) {
    this->closeConnection();
  } else {
    this->logger->warn( CONNECTION_NOT_ACTIVE );
    theMutex.lock();
    DEBUG_WARN( CONNECTION_NOT_ACTIVE );
    theMutex.unlock();
  }
}

void Client::shutdownConnection() {
  mutex theMutex;
  this->disconnectFromServer();
  this->logger->warn( CLIENT_CLOSE );
  theMutex.lock();
  DEBUG_WARN( CLIENT_CLOSE );
  theMutex.unlock();
  exit( 0 );
}

void Client::closeConnection() {
  mutex theMutex;
  close( this->socketFD );
  this->connected = false;
  this->logger->warn( CONNECTION_CLOSE );
  theMutex.lock();
  DEBUG_WARN( CONNECTION_CLOSE );
  theMutex.unlock();
}
