#include "Server.h"
#include "../transmitter/Transmitter.h"
#include "../socket/sock_dep.h" /* socket dependencies */
#include "../../xml/parser/XMLParser.h"
#include "../palette/palette.h"
#include "../../utils/K.h"
#define DEBUG 1
#include "../debug/dg_msg.h"
#include <regex>
#include <thread>
#include <mutex>
#include <iostream>
using namespace std;

Server::Server( const char* configFileName ) {
  this->socketFD = 0;
  this->clientCount = 0;
  this->listening = false;
  this->connected = false;
  this->processing = false;
  this->allowConnections = false;
  this->eventQueue = new queue<map<int, Evento*>*>;
  this->logger = Logger::instance();
  //  this->config = XMLParser::parseServerConf( configFileName );
}

Server::~Server() {
  // Delete players if any
  if( !( this->players.empty() ) ) {
    for( map<int, Player*>::iterator it = this->players.begin();
	 it != this->players.end();
	 ++it ) {
      delete it->second;
    }
  }

  delete this->eventQueue;
}

void Server::initialize() {
  if( this->connected ) {
    this->logger->warn( CONNECTION_ACTIVE );
    DEBUG_WARN( CONNECTION_ACTIVE );
    return;
  }

  int cfd; // client file descriptor
  struct sockaddr_storage client_addr; // client address information
  struct addrinfo hints, *servinfo, *p; // configuration structs
  int rv;

  // init hints struct with 0
  memset( &hints, 0, sizeof( hints ) );

  // set hints struct values
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use host IP

  // fill configuration structs
  // HARDCODEADO
  if( ( rv = getaddrinfo( NULL, to_string( 8000 ).c_str(), &hints, &servinfo ) ) != 0 ) {
    this->logger->error( "Error al obtener la direccion, " + string( gai_strerror( rv ) ) );
    exit( -1 );
  }

  int yes = 1;
  // loop through results and bind to one of them
  for( p = servinfo; p != NULL; p = p->ai_next ) {
    // try to create TCP socket
    if( ( this->socketFD = socket( p->ai_family, p->ai_socktype, p->ai_protocol ) ) == -1 ) {
      this->logger->error( SOCKET_ERROR );
      continue; // try next one
    }

    // allow port reuse to avoid bind error
    if( setsockopt( this->socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == -1 ) {
      this->logger->error( SOCKET_REUSE_ERROR );
      exit( -1 );
    }

    // bind socket
    if( bind( this->socketFD, p->ai_addr, p->ai_addrlen ) == -1 ) {
      close( this->socketFD );
      this->logger->error( BIND_ERROR );
      continue; // try next one
    }

    break; // socket created and binded
  }
  freeaddrinfo( servinfo ); // free memory

  if( p == NULL ) {
    this->logger->error( BIND_CERROR );
    exit( -1 );
  }

  // listen for connections
  if( listen( this->socketFD, this->BACKLOG ) == -1 ) {
    this->logger->error( LISTEN_ERROR );
    exit( -1 );
  }

  this->listening = true;
  this->connected = true;
  this->processing = true;
  thread processor( &Server::processQueue, this );
  processor.detach();
  this->logger->info( SERVER_START );
  DEBUG_NOTICE( SERVER_START );

  // accept connections
  thread t2( &Server::listenForConnections, this, cfd, client_addr );
  t2.detach();
}

void Server::listenForConnections( int cfd, struct sockaddr_storage client_addr ) {
  // accept connections
  while( this->listening ) {
    socklen_t sinSize = sizeof client_addr;
    if( ( cfd = accept( this->socketFD, (struct sockaddr*) (&client_addr), &sinSize ) ) == -1 ) {
      this->logger->error( "Error al aceptar Cliente" );
      exit( -1 );
    }
    this->clientCount++;

    //HARDCODEADO
    this->allowConnections = ( this->clientCount <= 4 );

    thread tCheckAliveSend( &Server::checkAliveSend, this, cfd);
    tCheckAliveSend.detach();

    thread process( &Server::receiveClientData, this, cfd, client_addr );
    process.detach();
  }
}

// get sockaddr, IPv4 
void* Server::getInAddr( struct sockaddr* sa ) {
  if( sa->sa_family == AF_INET ) {
    return &( ( (struct sockaddr_in*) sa )->sin_addr );
  }
}

void Server::addPlayer( PlayerData* data, int cfd ) {
  string validName = "Y", validColor = "Y";
  mutex theMutex;
  string selectedName( data->name );
  string selectedColor( data->color );
  bool createPlayer = true;

  theMutex.lock();
  for( map<int, Player*>::iterator it = this->players.begin();
       it != this->players.end();
       ++it ) {
    // if already a player with that name
    if( selectedName == it->second->getName() ) {
      createPlayer = false;
      validName = "N";
      // if player with such name is not active
      if( !( it->second->isActive() ) ) {
	// resume player game
	selectedColor = it->second->getColor();
	this->players.erase( it );
	createPlayer = true;
	validName = "R";
	validColor = "R";
	break;
      } 
    }
    // if already a player with that color 
    if( selectedColor == it->second->getColor() ) {
      createPlayer = false;
      validColor = "N";
    }
  }
  theMutex.unlock();

  if( createPlayer ) {
    // Add new player
    Player* p = new Player( selectedName, selectedColor );
    theMutex.lock();
    this->players[ cfd ] = p;
    theMutex.unlock();
  }

  // Create response
  PlayerData* response = new PlayerData;
  // Fill response struct
  strcpy( response->name, validName.c_str() );
  strcpy( response->color, validColor.c_str() );

  Transmitter* tmt = new Transmitter( cfd, this->logger );
  if( !( tmt->sendData( response ) ) ) {
    DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::addPlayer" );
    this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::addPlayer" );
  }

  delete response;
  delete tmt;
}


void Server::sendPlanesActives(int cfd){

	  PlanesActives* planes = new PlanesActives;
	  planes->blue = true;
	  planes->red = true;
	  planes->green = true;
	  planes->yellow = true;

	  mutex theMutex;
	  theMutex.lock();
	  for( map<int, Player*>::iterator it = this->players.begin(); it != this->players.end();  ++it ) {
	    // if already a player with that color
		  if( it->second->getColor() == "azul" ) {
			  planes->blue = false;
	      } else if( it->second->getColor() == "rojo" ) {
		      planes->red = false;
		  } else if( it->second->getColor() == "verde" ) {
			  planes->green = false;
		  } else if( it->second->getColor() == "amarillo" ) {
		      planes->yellow = false;
		  }
	  }
	  theMutex.unlock();
	  Transmitter* tmt = new Transmitter( cfd, this->logger );
	  if( !( tmt->sendData( planes ) ) ) {
	    DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::addPlayer" );
	    this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::addPlayer" );
	  }

	  delete planes;
	  delete tmt;
}

void Server::receiveClientData( int cfd, struct sockaddr_storage client_addr ) {
  char clientIP[ INET_ADDRSTRLEN ]; // connected client IP
  Evento* msgToRecv = new Evento;

  // get connected host IP in presentation format
  inet_ntop( client_addr.ss_family,
      this->getInAddr( (struct sockaddr*) (&client_addr) ), clientIP,
      sizeof clientIP);

  if( this->allowConnections ) {
    cout << endl << notice( "Se inicio una conexion con el host: " ) << clientIP
      << endl;
    this->logger->info( "Se inicio una conexion con el host: " + string( clientIP ) );

    if( send( cfd, "Aceptado", 8, 0 ) == -1 ) {
      this->logger->error( "Error al enviar que se acepto la conexion" );
    }

    usleep(1);
    this->sendPlanesActives( cfd);

    timeval timeout;
    timeout.tv_sec = this->MAX_UNREACHABLE_TIME;
    timeout.tv_usec = 0;
    bool receiving = true;
    bool received;
    char id[3];
    // Create transmitter for this client
    Transmitter* tmt = new Transmitter( cfd, this->logger );

    while( receiving ) {
      // seteo el timeout de recepcion de mensajes
      if( setsockopt( cfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout, sizeof( timeout ) ) < 0 ) {
	cout << "Error sockopt" << endl;
	exit( 1 );
      }

      // Get id of next data to receive
      received = tmt->receiveData( id, sizeof( id ) );

      if( received ) {
	string dataID( id );
	// Receive data type based on fetched dataID 
	if( dataID == "PD" ) {
	  PlayerData* data = new PlayerData;

	  if( received = tmt->receiveData( data ) ) {
	    // Process received data
	    cout << "Nombre del jugador: " << string( data->name ) << endl;
	    cout << "Color del jugador: " << string( data->color ) << endl;
	    this->addPlayer( data, cfd );
	  }

	  delete data;
	} 
      }

      if( !( received ) ) {
	receiving = false;
	cout << endl << warning( "El cliente " ) << clientIP
	  << warning( " se desconecto" ) << endl;
	this->logger->warn( "El Cliente " + string( clientIP ) + " se desconecto" );
	this->closeClient( cfd );
      }

      // Read data
      /*if( ( numBytesRead = recv( cfd, msgToRecv, sizeof( Mensaje ), 0 ) ) == -1 ) {
	close( cfd );
	this->logger->warn( CONNECTION_TIMEOUT );
	DEBUG_WARN( CONNECTION_TIMEOUT );
	}

	if( numBytesRead > 0 ) {
	if( numBytesRead != 1 ) {
	theMutex.lock();
	cout << endl << "FD cliente: " << notice( to_string( cfd ) ) << endl;
	cout << "Evento recibido: " << notice( to_string(msgToRecv->value) ) << endl;
	cout << "Jugador que envio el evento: " << notice( to_string(msgToRecv->playerID) ) << endl;

	map<int,Evento*>* clientMsgFD = new map<int,Evento*>();
	clientMsgFD->insert( pair<int,Evento*>( cfd, msgToRecv ) );
	this->eventQueue->push( clientMsgFD );
	theMutex.unlock();
	}
	} else {
	receiving = false;
	cout << endl << warning( "El cliente " ) << clientIP
	<< warning( " se desconecto" ) << endl;
	this->logger->warn( "El Cliente " + string( clientIP ) + " se desconecto" );
	this->closeClient( cfd );
	}*/
    }

    delete tmt;
  } else {
    cout << endl << warning( "El cliente " ) << clientIP << warning( " se rechazo" ) << endl;
    this->logger->warn( "El cliente " + string(clientIP) + " se rechazo" );
    usleep( 1000000 );
    this->closeClient( cfd );
  }
}

void Server::checkAliveSend( int cfd ) {
  char buf[1] = { '1' };

  while( true ) {
    if( !( this->connected ) ) return;
    // 4s timed send
    usleep(4000000);
    send( cfd, &buf, 1, 0 );
  }
}

void Server::processQueue() {
  bool msgIsValid;
  Evento* respuesta = new Evento;
  mutex theMutex;

  while( this->processing ) {
    if( !( this->eventQueue->empty() ) ) {
      theMutex.lock();
      //cout << "Saco Msj de la cola" << endl;
      map<int,Evento*>* data = this->eventQueue->front();
      this->eventQueue->pop();

      map<int,Evento*>::iterator it = data->begin();
      //cout << "FD cliente: " << it->first << " --  Mensaje: " << (it->second)->valor << endl;

      this->logger->info( "Msj de cliente: " + to_string(it->second->value ) );

      //      msgIsValid = this->processMsg( string((it->second)->tipo), string(((it->second)->valor)) );
      //      if( msgIsValid ) {
      respuesta->value = MENSAJE_CORRECTO;
      this->logger->info( to_string(respuesta->value) );
      //      } else {
      //	respuesta->value = MENSAJE_INCORRECTO;
      //	this->logger->warn( respuesta->value );
      //      }
      thread tSending( &Server::sendData, this, it->first, respuesta , sizeof(Evento) );
      tSending.detach();

      delete data;

      theMutex.unlock();
    }
  }
  //cout<<"Corto processor"<<endl;
  delete respuesta;
}

//bool Server::processMsg( string tipo, string valor ){
//  const int MAX_INT = 2147483647;
//  bool respuesta = false;
//  regex r;
//  const char* expr;
//
//  if( tipo == K::typeInt ){
//    //expr = "^-?(2?1?[0-4]?|2?0?[0-9]?|[0-1]?[0-9]?[0-9]?)([0-9]){1,7}$";//menor que +-2148000000
//    expr = "^-?[0-9]+$";
//    r = regex(expr);
//    if((regex_match(valor, r)) && (atoi(valor.c_str()) >= -MAX_INT) && (atoi(valor.c_str()) <= MAX_INT)) //ese casteo de char* a int no se si se puede
//      respuesta = true;
//
//  } else {
//
//    if( tipo == K::typeDouble ){
//      expr = "^-?([0-9]+e-?[//+]?[0-9]{1,3}|[0-2][//.][0-9]{0,2}e-?[//+]?[0-9]{1,3}|[0-9]+[//.][0-9]+)$";
//      r = regex(expr);
//      if (regex_match(valor, r)) respuesta = true;
//
//    } else {
//
//      if( tipo == K::typeString ){
//	expr = "^.+$";
//	r = regex(expr);
//	if( regex_match( valor, r ) ) respuesta = true;
//
//      } else {
//
//	if( tipo == K::typeChar ){
//	  expr = "^.$";
//	  r = regex(expr);
//	  if( regex_match( valor, r ) ) respuesta = true;
//	}
//      }
//    }
//  }
//  return respuesta;
//}

void Server::sendData( int cfd, Evento* data, int dataLength ){
  if( send( cfd, data, dataLength, 0 ) == -1 ) {
    this->logger->warn( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
  }
}

void Server::closeClient( int cfd ) {
  mutex theMutex;
  close( cfd );
  theMutex.lock();
  this->clientCount--;
  cout << " cantidad " << this->clientCount << endl;
  this->logger->info( "Cantidad de Clientes Conectados: " + to_string( this->clientCount ) );
  theMutex.unlock();
}

void Server::shutdown() {
  if( this->connected ) 
    this->closeConnection();

  logger->warn( SERVER_CLOSE );
  DEBUG_WARN( SERVER_CLOSE );
  exit( 0 );
}

void Server::closeConnection() {
  close( this->socketFD );
  this->listening = false;
  this->connected = false;
  this->processing = false;
  this->logger->warn( SERVER_DISCONNECT );
  DEBUG_WARN( SERVER_DISCONNECT );
}
