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
#include "../../xml/parser/GameParser.h"
#include "../../xml/parser/XMLParser.h"

using namespace std;

Server::Server( const char* configFileName ) {
  this->config = GameParser::parse("gameconf.xml");
  this->socketFD = 0;
  this->clientCount = 0;
  this->maxClientCount = this->config->maxClients;
  this->listening = false;
  this->connected = false;
  this->processing = false;
  this->allowConnections = false;
  this->eventQueue = new queue<map<int, Evento*>*>;
  this->logger = Logger::instance();
  this->running = false;
  this->stageData = NULL;

  this->posicionInicialX = 0;
  this->posicionInicialY = 100;
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
    this->allowConnections = ( this->clientCount <= this->maxClientCount );

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

void Server::updatePlayerStatus( PlayerStatus* data, int cfd ) {
  if( data->status == 'D' ) {
    this->players[ cfd ]->deactivate();
  }
}

void Server::addPlayer(PlayerData* data, int cfd) {
	string validName = "Y", validColor = "Y";
	mutex theMutex;
	string selectedName(data->name);
	string selectedColor(data->color);
	int selectedTeam = data->team;
	bool createPlayer = true;
	bool encontrePlayer = false;
	theMutex.lock();

	for (map<int, Player*>::iterator it = this->players.begin() ; it != this->players.end() ; ++it) {
		// if already a player with that name
		if (selectedName == it->second->getName()) {
			createPlayer = false;
			validName = "N";
			cout<<"Encuentro Player"<<endl;
			// if running game and player with such name is not active
			if ( this->running && !(it->second->isActive())) {
				// resume player game
				cout<<"Resume Game"<<endl;
				selectedColor = it->second->getColor();
				posicionInicialX = it->second->getX();
				posicionInicialY = it->second->getY();
				delete it->second;
				this->players.erase(it);
				createPlayer = true;
				validName = "R";
				validColor = "R";
				break;
			}
		}

		// if already a player with that color
		if (selectedColor == it->second->getColor()) {
			createPlayer = false;
			validColor = "N";
		}
	}
	theMutex.unlock();

	if (createPlayer && (this->players.size() < this->maxClientCount) ) {
		// Add new player
		cout<<"Creo Jugador"<<endl;
		Player* p = new Player(selectedName, selectedColor, posicionInicialX, posicionInicialY, selectedTeam);
		theMutex.lock();
		this->players[cfd] = p;
		posicionInicialX += 100;
		theMutex.unlock();
	} else {
		cout<<"No creo jugador"<<endl;
		createPlayer = false;
			validName = "N";
			validColor = "N";
	}
	// Create response
	PlayerData* response = new PlayerData;
	// Fill response struct
	strcpy(response->name, validName.c_str());
	strcpy(response->color, validColor.c_str());

	cout<<"name :"<<validName<<"  .Color: "<<validColor<<endl;
	Transmitter* tmt = new Transmitter(cfd, this->logger);
	if (!(tmt->sendData(response))) {
		DEBUG_WARN("No se pude enviar respuesta a cliente. JOB: Server::addPlayer");
		this->logger->error("No se pude enviar respuesta a cliente. JOB: Server::addPlayer");
	}
	delete response;
	delete tmt;

	theMutex.lock();
	if (createPlayer && this->players.size() == this->maxClientCount) {
		cout << "send players" << endl;
		this->createPlayers();
		if( !( this->running ) ) this->running = true;
	}
	theMutex.unlock();
}

void Server::queryCurrentStageOffset() {
  if( this->stageData != NULL ) {  
    delete this->stageData;
    this->stageData = NULL;
  }

  for( map<int, Player*>::iterator it = this->players.begin();
      it != this->players.end(); ++it ) {
    if( it->second->isActive() ) {
      Transmitter* tmt = new Transmitter( it->first, this->logger );
      tmt->sendDataID( "SQ" );
      delete tmt;
      break;
    }
  }
}

void Server::sendCurrentStageOffset( int clientFD ) {
  while( this->stageData == NULL );
  Transmitter* tmt = new Transmitter( clientFD, this->logger );
  tmt->sendData( this->stageData, "SD" );
  delete tmt;
}

void Server::createPlayers() {
  map<int, Player*>::iterator it2 = this->players.begin();

  for (int i = 0; i < this->players.size(); i++) {
    // if player not active
    if( !( it2->second->isActive() ) ) {
      // send stage config
      this->sendConf(it2->first);

      // if game already started
      if( this->running ) {
	// get stage offset
	this->queryCurrentStageOffset();

	// send stage offset to player
	this->sendCurrentStageOffset( it2->first );
      }
    }
    // send other players data
    for (map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end(); ++it) {
      Transmitter* tmt = new Transmitter(it2->first, this->logger);

      PlayerData* player = new PlayerData;
      strcpy(player->name, it->second->getName().c_str());
      strcpy(player->color, it->second->getColor().c_str());
      player->x = it->second->getX();
      player->y = it->second->getY();
      player->team = it->second->getTeam();

      while (!tmt->sendData(player, "PR"));

      delete player;
      delete tmt;
    }
    // activate player
    it2->second->activate();
    it2++;
  }
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
    // if game is running and player is inactive, skip 
    if( this->running && !( it->second->isActive() ) ) continue;
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


void Server::sendConf(int cfd){
  this->config = GameParser::parse("gameconf.xml");
  Transmitter* tmt = new Transmitter( cfd, this->logger );

  AvionConf* avion = this->config->avion;
  if( !( tmt->sendData( avion ) ) ) {
    DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send avion" );
    this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send avion" );
  }

  EscenarioConf* escenario = this->config->escenario;
  if( !( tmt->sendData( escenario ) ) ) {
    DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send escenario" );
    this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send escenario" );
  }

  vector<ElementoConf*> elementos = this->config->elementos;
  for (int var = 0; var < elementos.size(); ++var) {
    ElementoConf* elemento = elementos[var];
    if( !( tmt->sendData( elemento ) ) ) {
      DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send elementos" );
      this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send elementos" );
    }
  }

  vector<SpriteConf* > sprites = this->config->sprites;
  for (int var = 0; var < sprites.size(); ++var) {
    SpriteConf* sprite = sprites[var];
    if( !( tmt->sendData( sprite ) ) ) {
      DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send Sprites" );
      this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send Sprites" );
    }
  }

  if( !( tmt->sendEndDataConf(this->clientCount)) ) {
    DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::sendConf" );
    this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::sendConf" );
  }
  cout<<"Envio toda la Configuracion"<<endl;
  delete tmt;
}


void Server::receiveClientData( int cfd, struct sockaddr_storage client_addr ) {
  char clientIP[ INET_ADDRSTRLEN ]; // connected client IP
  Evento* msgToRecv = new Evento;
  mutex theMutex;

  // get connected host IP in presentation format
  inet_ntop( client_addr.ss_family,
      this->getInAddr( (struct sockaddr*) (&client_addr) ), clientIP,
      sizeof clientIP);

  if( this->allowConnections ) {
    //players2.push_back(cfd);
    cout << endl << notice( "Se inicio una conexion con el host: " ) << clientIP
      << endl;
    this->logger->info( "Se inicio una conexion con el host: " + string( clientIP ) );

    if( send( cfd, "Aceptado", 8, 0 ) == -1 ) {
      this->logger->error( "Error al enviar que se acepto la conexion" );
    }

    usleep(100);
    this->sendPlanesActives( cfd);

    timeval timeout;
    timeout.tv_sec = this->MAX_UNREACHABLE_TIME;
    timeout.tv_usec = 0;
    bool receiving = true;
    int bytesReceived;
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
      bytesReceived = tmt->receiveData( id, sizeof( id ));

      if( bytesReceived > 0 ) {
	string dataID( id );
	// Receive data type based on fetched dataID 
	if( dataID == "PD" ) {
	  PlayerData* data = new PlayerData;

	  if( ( bytesReceived = tmt->receiveData( data )) > 0 ) {
	    // Process received data
	    cout << "Nombre del jugador: " << string( data->name ) << endl;
	    cout << "Color del jugador: " << string( data->color ) << endl;
	    cout << "Equipo del jugador: " << data->team << endl;
	    this->addPlayer( data, cfd );
	  }

	  delete data;
	} else if( dataID == "PS" ) {
	  PlayerStatus* data = new PlayerStatus;

	  if( ( bytesReceived = tmt->receiveData( data )) > 0 ) {
	    // Process received data
	    cout << "Player status: " << data->status << endl;
	    this->updatePlayerStatus( data, cfd );
	  }

	  delete data;
	} else if (dataID == "EV") {
	  Evento* e = new Evento();

	  if (( bytesReceived = tmt->receiveData(e)) > 0 ) {
	    cout << "Evento: " << e->value << endl;

	    theMutex.lock();
	    cout << endl << "FD cliente: " << notice(to_string(cfd)) << endl;

	    map<int, Evento*>* clientMsgFD = new map<int, Evento*>();
	    clientMsgFD->insert(pair<int, Evento*>(cfd, e));
	    this->eventQueue->push(clientMsgFD);
	    theMutex.unlock();
	  }

	} else if (dataID == "DP") {
	  PlayerData* data = new PlayerData;

	  if( ( bytesReceived = tmt->receiveData( data ) ) > 0 ) {
	    for (map<int, Player*>::iterator it = this->players.begin(); it != this->players.end(); ++it) {
	      if ( data->name == (it->second)->getName() ){
		Player* p = it->second;
		p->setX(data->x);
		p->setY(data->y);
		//cout<< "Jugador: " << p->getName() << endl<< "Color: " << p->getColor() << endl<<"Posiciones :" << p->getX() << "    "<< p->getY()<<endl;
		break;
	      }
	    }
	  }

	  delete data;
	} else if(dataID == "CO" ){
	  cout<<"Reset cliente "<<cfd<<endl;
	  this->sendConf(cfd);
	} else if( dataID == "SD" ) {
	  StageData* data = new StageData;

	  if( ( bytesReceived = tmt->receiveData( data ) ) > 0 ) {
	    // Process received data
	    cout << "Current stage offset: " << data->offset << endl;
	    this->stageData = data;
	  }
	} else if( dataID == "SP" ) {
	  PlayerScore* data = new PlayerScore;
	  if( ( bytesReceived = tmt->receiveData( data ) ) > 0 ) {
	    // Process received data
	    this->sendScoreData( data );
	  }
	}

      }

      // Check peer disconnection or timeout
      if ( bytesReceived <= 0 ) {
	receiving = false;
	this->avisarDesconexionDeAvion(cfd);
	this->closeClient( cfd );
	if( bytesReceived == 0 ) {
	  cout << endl << warning( "El cliente " ) << clientIP
	    << warning( " se desconecto" ) << endl;
	  this->logger->warn( "El Cliente " + string( clientIP ) + " se desconecto" );
	} else {
	  DEBUG_WARN( CONNECTION_TIMEOUT );
	  this->logger->warn( CONNECTION_TIMEOUT );
	}
      }
    }

    delete tmt;
  } else {
    cout << endl << warning( "El cliente " ) << clientIP << warning( " se rechazo" ) << endl;
    this->logger->warn( "El cliente " + string(clientIP) + " se rechazo" );
    usleep( 1000000 );
    this->closeClient( cfd );
  }
}

void Server::avisarDesconexionDeAvion(int cfd) {
  CompanionEvent* ce = new CompanionEvent();
  string disconnectedPlayerName = this->players[cfd]->getName();

  if (!(this->players.empty())) {
    for (map<int, Player*>::iterator itP = this->players.begin(); itP != this->players.end(); ++itP) {
      if ((itP->first) != cfd) {
	sendData(itP->first, ce->quit(disconnectedPlayerName));
      }
    }
  }
  delete ce;
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

  while (this->processing) {
    if (!(this->eventQueue->empty())) {
      theMutex.lock();
      //cout << "Saco Msj de la cola" << endl;
      map<int, Evento*>* data = this->eventQueue->front();
      this->eventQueue->pop();

      map<int, Evento*>::iterator it = data->begin();
      cout << "FD cliente: " << it->first << " --  Mensaje: "
	<< (it->second)->value << endl;

      this->logger->info("Msj de cliente: " + to_string(it->second->value));

      //      msgIsValid = this->processMsg( string((it->second)->tipo), string(((it->second)->valor)) );
      //      if( msgIsValid ) {
      respuesta->value = MENSAJE_CORRECTO;
      this->logger->info(to_string(respuesta->value));
      //      } else {
      //	respuesta->value = MENSAJE_INCORRECTO;
      //	this->logger->warn( respuesta->value );
      //      }
      //thread tSending( &Server::sendData, this, it->first, respuesta , sizeof(Evento) );
      //tSending.detach();


      //TODO: Aqui vendria el codigo para eliminar todos los aviones enemigos
      if (it->second->value == 'X') {
      	cout << "Destruyo todos los aviones enemigos en la Queue, avisando a todos los clientes de este suceso " <<endl;
      } else {
  			if (!(this->players.empty())) {
  				for (map<int, Player*>::iterator itP = this->players.begin();
  						itP != this->players.end(); ++itP) {
  					if ((itP->first) != it->first) {
  						sendData(itP->first, it->second);
  					}
  				}
  			}
      }

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

/*
   void Server::sendData( int cfd, Evento* data, int dataLength ){
   if( send( cfd, data, dataLength, 0 ) == -1 ) {
   this->logger->warn( SEND_FAIL );
   DEBUG_WARN( SEND_FAIL );
   }
   }*/

void Server::sendData( int cfd, Evento* data ) {
  Transmitter* tmt = new Transmitter( cfd, this->logger );
  tmt->sendData( data );
}

void Server::closeClient( int cfd ) {
  mutex theMutex;
  close( cfd );
  theMutex.lock();
  this->clientCount--;
  // if no more players connected
  if ( this->clientCount == 0 ) {
    // clear players hash
    this->removeAllPlayers();
    this->running = false;
  }
  cout << " cantidad " << this->clientCount << endl;
  this->logger->info( "Cantidad de Clientes Conectados: " + to_string( this->clientCount ) );
  theMutex.unlock();
}

void Server::removeAllPlayers() {
  for( map<int, Player*>::iterator it = this->players.begin();
       it != this->players.end();
       ++it ) {
      delete it->second;
  }
  this->players.clear();
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

void Server::sendScoreData( PlayerScore* data ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Transmitter* tmt = new Transmitter( it->first, this->logger ); 
    tmt->sendData( data );
    delete tmt;
  }
}
