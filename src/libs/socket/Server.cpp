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
  this->readyPlayers = 0;
  this->maxClientCount = this->config->maxClients;
  this->listening = false;
  this->connected = false;
  this->processing = false;
  this->eventQueue = new queue<map<int, Evento*>*>;
  this->logger = Logger::instance();
  this->running = false;
  this->stageData = NULL;
  this->posicionInicialX = 0;
  this->posicionInicialY = 100;
  this->createGameData();
  //  this->config = XMLParser::parseServerConf( configFileName );
  this->alphaTeamScore = 0;
  this->betaTeamScore = 0;
  this->coopTeamScore = 0;
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
  socklen_t sinSize = sizeof client_addr;
 
  while( this->listening ) {
    char clientIP[ INET_ADDRSTRLEN ]; // connected client IP
    
    // accept connection
    if( ( cfd = accept( this->socketFD, (struct sockaddr*) (&client_addr), &sinSize ) ) == -1 ) {
      this->logger->error( "Error al aceptar Cliente" );
      continue;
    }
    this->clientCount++;

    // if client slots left 
    if ( this->clientCount <= this->maxClientCount ) {
      // get connected host IP in presentation format
      inet_ntop( client_addr.ss_family, 
		 this->getInAddr( (struct sockaddr*) (&client_addr) ), clientIP,
		 sizeof clientIP);

      // notify client of established connection
      cout << endl << notice( "Se inicio una conexion con el host: " ) << clientIP
	   << endl;
      this->logger->info( "Se inicio una conexion con el host: " + string( clientIP ) );

      if( send( cfd, "Aceptado", 8, 0 ) == -1 ) {
	this->logger->error( "Error al enviar que se acepto la conexion" );
      }

      usleep( 100 );
      // send used planes and game data      
      this->sendPlanesActives( cfd );
      this->sendGameData( cfd );

      // create timeout check thread
      thread tCheckAliveSend( &Server::checkAliveSend, this, cfd );
      tCheckAliveSend.detach();

      // create thread for receiving client data
      thread process( &Server::receiveClientData, this, cfd, clientIP );
      process.detach();
    } else {
      cout << endl << warning( "El cliente " ) << clientIP << warning( " se rechazo" ) << endl;
      this->logger->warn( "El cliente " + string(clientIP) + " se rechazo" );
      //usleep( 1000000 );
      this->closeClient( cfd );
    }
  }
}

// get sockaddr, IPv4 
void* Server::getInAddr( struct sockaddr* sa ) {
  if( sa->sa_family == AF_INET ) {
    return &( ( (struct sockaddr_in*) sa )->sin_addr );
  }
}

void Server::updatePlayerStatus( PlayerStatus* data, int cfd ) {
  if( data->status == 'I' ) {
    this->players[ cfd ]->deactivate();
  } else if ( data->status == 'D' ) {
    this->players[ cfd ]->die();
    // check if remaining players win or loose
    if ( this->gameData->teamMode ) {
      this->checkTeamWin();
    } else {
      this->checkCoopLose();
    }
  }
  if (data->status == 'R' ){
	this->players[ cfd ]->changeReady();
	cout<<"Estado: cantidad de players - "<< this->players.size()<<endl;
	if(this->players.size() == this->maxClientCount){
		this->sendPlayersReady();
	}
  }
}

void Server::addPlayer(PlayerData* data, int cfd) {
	string validName = "Y", validColor = "Y";
	mutex theMutex;
	string selectedName(data->name);
	string selectedColor(data->color);
	int selectedTeam = data->team;
	int score = 0;
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
				cout << "RESUME SCORE: " << it->second->getScore() << endl;
				score = it->second->getScore();
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
		p->addScore( score );
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
      Player* player = it->second;
      Transmitter* tmt = new Transmitter(it2->first, this->logger);

      PlayerData* pd = new PlayerData;
      strcpy(pd->name, player->getName().c_str());
      strcpy(pd->color, player->getColor().c_str());
      pd->x = player->getX();
      pd->y = player->getY();
      pd->team = player->getTeam();
      pd->score = player->getScore();

      if ( it2->second->getName() == player->getName() ) {
	// get team score
	if ( this->gameData->cooperativeMode ) {
	  pd->teamScore = this->getPlayerTeamScore( player );
	// get team score and rival team score
	} else if ( this->gameData->teamMode ) {
	  pd->teamScore = this->getPlayerTeamScore( player );
	  pd->rivalTeamScore = this->getPlayerRivalTeamScore( player );
	}
      }

      while (!tmt->sendData(pd, "PR"));

      delete pd;
      delete tmt;
    }
    // activate player
    it2->second->activate();
    it2++;
  }
}

int Server::getPlayerTeamScore( Player* player ) {
  int teamScore = 0;
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    if ( player->getTeam() == it->second->getTeam() ) {
      teamScore += it->second->getScore();
    }
  }

  return teamScore;
}

int Server::getPlayerRivalTeamScore( Player* player ) {
  int rivalTeamScore = 0;
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    if ( player->getTeam() != it->second->getTeam() ) {
      rivalTeamScore += it->second->getScore();
    }
  }

  return rivalTeamScore;

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

  vector<PowerUpConf*> powerUps = this->config->powerUps;
    for (int var = 0; var < powerUps.size(); ++var) {
    	PowerUpConf* powerUp = powerUps[var];
      if( !( tmt->sendData( powerUp ) ) ) {
        DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send powerUps" );
        this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send powerUps" );
      }
    }

    vector<EnemigoConf*> enemigos = this->config->enemigos;
    for (int var = 0; var < enemigos.size(); var++) {
    	EnemigoConf* enemigo = enemigos[var];
    	if (!tmt->sendData(enemigo)) {
    		DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send enemigos" );
			this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send enemigos" );
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
//  cout<<"Envio toda la Configuracion"<<endl;
  delete tmt;
}


void Server::receiveClientData( int cfd, string clientIP ) {
  Evento* msgToRecv = new Evento;
  mutex theMutex;
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
	    //Modo Practica
	    if (e->value == 'O'){
	    	this->gameData->practiceMode = false;
	    }
	    theMutex.lock();
	    cout << endl << "FD cliente: " << notice(to_string(cfd)) << endl;
	    map<int, Evento*>* clientMsgFD = new map<int, Evento*>();
	    clientMsgFD->insert(pair<int, Evento*>(cfd, e));
	    this->eventQueue->push(clientMsgFD);
	    theMutex.unlock();
	  }
	} else if ( dataID == "QG" ) {
	    // free player slot
	    this->freePlayerSlot( cfd );
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
	    // add score to corresponding player 
	    this->addScoreToPlayer( data );
	  }
	  delete data;
	} else if( dataID == "PQ" ) {
	  // send active players count
	  this->sendActivePlayers( cfd );
	} else if( dataID == "RR" ) {
	  // increment player ready count
	  this->readyPlayers++; 
	  if ( this->readyPlayers == this->getActivePlayersCount() ) {
	    this->sendStageReadySignal();
	    this->readyPlayers = 0;
	  }
	} else if( dataID == "ST" ) {
	  // send score table
	  this->sendScoreTable( cfd );
	} else if( dataID == "RS" ) {
	  // reset score
	  this->players[ cfd ]->resetScore();
	} else if( dataID == "GD" ) {
	  GameData* data = new GameData;
	  if( ( bytesReceived = tmt->receiveData( data ) ) > 0 ) {
		// Process received data
		this->gameData = data;
		cout<<"cantidad team 1 "<<this->gameData->countPlayersTeam1<<endl;
		cout<<"cantidad team 2 "<<this->gameData->countPlayersTeam2<<endl;
	    this->sendGameData( cfd );
	  }
	} else if( dataID == "MT" ) {
		if(!this->gameData->cooperativeMode && !this->gameData->teamMode){
			this->gameData->teamMode = true;
//			cout<<"modo team"<<endl;
			this->sendGameDataAll();
		}
	} else if( dataID == "MC" ) {
		if(!this->gameData->cooperativeMode && !this->gameData->teamMode){
			this->gameData->cooperativeMode = true;
//			cout<<"modo cooperativo"<<endl;
			this->setTeamPlayer(0, cfd);
			this->sendGameDataAll();
		}
	} else if( dataID == "MP" ) {
		if(!this->gameData->practiceMode){
			this->gameData->practiceMode = true;
//			cout<<"modo cooperativo"<<endl;
			this->sendGameDataAll();
		}
	} else if( dataID == "T1" ) {
		cout<<"se suma al team 1"<<endl;
		this->gameData->countPlayersTeam1++;
		this->setTeamPlayer(1, cfd);
		this->sendGameDataAll();
	} else if( dataID == "T2" ) {
		cout<<"se suma al team 2"<<endl;
		this->gameData->countPlayersTeam2++;
		this->setTeamPlayer(2, cfd);
		this->sendGameDataAll();
	}
      }

      // Check peer disconnection or timeout
      if ( bytesReceived <= 0 ) {
	receiving = false;

	if ( this->running && ( this->clientCount > 1 ) && ( this->players.find( cfd ) != this->players.end() ) ) {
	  thread timedDisconnection( &Server::checkAliveClose, this, cfd );
	  timedDisconnection.detach();
	} else {
	  this->closeClient( cfd );
	}

	// disconnection	
	if( bytesReceived == 0 ) {
	  cout << endl << warning( "El cliente " ) << clientIP
	       << warning( " se desconecto" ) << endl;
	  this->logger->warn( "El Cliente " + string( clientIP ) + " se desconecto" );
	// timeout
	} else {
	  DEBUG_WARN( CONNECTION_TIMEOUT );
	  this->logger->warn( CONNECTION_TIMEOUT );
	}
      }
    }
}

void Server::checkAliveClose( int clientFD ) {
  this->avisarDesconexionDeAvion( clientFD );
  this->closeClient( clientFD );
  this->players[ clientFD ]->deactivate();
  // 10 sec wait
  usleep( 10000000 );
  // if player didn't resume
  if ( this->players.find( clientFD ) != this->players.end() ) {
    this->freePlayerSlot( clientFD );
  }
}

void Server::avisarDesconexionDeAvion(int cfd) {
  // if player isn't playing yet, skip
  map<int, Player*>::iterator it = this->players.find( cfd );
  if( it == this->players.end() ) return;
  
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


			if (!(this->players.empty())) {
				for (map<int, Player*>::iterator itP = this->players.begin();
						itP != this->players.end(); ++itP) {
					if ((itP->first) != it->first) {
						sendData(itP->first, it->second);
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
    delete this->gameData;
    this->createGameData();
    this->running = false;
    // reset scores
    this->alphaTeamScore = 0;
    this->betaTeamScore = 0;
    this->coopTeamScore = 0;
  }
  cout << "Cantidad de clientes conectados: " << this->clientCount << endl;
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

void Server::addScoreToPlayer( PlayerScore* data ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    if ( it->second->getName() == string( data->name ) ) {
      it->second->addScore( data->score );
    } else if( it->second->isActive() ) {
      // send team score
      Transmitter* tmt = new Transmitter( it->first, this->logger );
      tmt->sendData( data, "TS" ); 
      delete tmt;
    }
  }

  if ( this->gameData->teamMode ) {
    // alpha team
    if ( data->team == 1 ) {
      this->alphaTeamScore += data->score;
    // beta team
    } else {
      this->betaTeamScore += data->score;
    }
  } else {
    this->coopTeamScore += data->score;
  }
}

void Server::sendScoreTable( int clientFD ) {
  Transmitter* tmt = new Transmitter( clientFD, this->logger );

  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Player* player = it->second;
    // create player score data
    PlayerScore* ps = new PlayerScore;
    strcpy( ps->name, ( player->getName() ).c_str() );
    strcpy( ps->color, ( player->getColor() ).c_str() );
    ps->score = player->getScore();
    ps->team = player->getTeam();

    // send score data
    tmt->sendData( ps );
    delete ps;
  }

  if ( this->gameData->teamMode ) {
    PlayerScore* as = new PlayerScore;
    PlayerScore* bs = new PlayerScore;
    strcpy( as->name, "alphaTotal" ); 
    as->score = this->alphaTeamScore;
    strcpy( bs->name, "betaTotal" ); 
    bs->score = this->betaTeamScore;
    
    tmt->sendData( as );
    tmt->sendData( bs );

    delete as;
    delete bs;
  } else {
    PlayerScore* ts = new PlayerScore;
    strcpy( ts->name, "total" );
    ts->score = this->coopTeamScore;
    tmt->sendData( ts );

    delete ts;
  }

  delete tmt;
}

void Server::sendActivePlayers( int clientFD ) {
  ActivePlayers* data = new ActivePlayers;
  data->playerCount = this->players.size();

  Transmitter* tmt = new Transmitter( clientFD, this->logger );
  tmt->sendData( data );

  delete tmt;
  delete data;
}

void Server::sendStageReadySignal() {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Transmitter* tmt = new Transmitter( it->first, this->logger ); 
    tmt->sendDataID( "RR" );
    delete tmt;
  }
}

int Server::getActivePlayersCount() {
  int playerCount = 0; 
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Player* player = it->second;
    // if player is connected and alive
    if ( player->isActive() && player->isAlive() ) {
      playerCount++;
    }
  }

  return playerCount;
}

void Server::sendGameData(int clientFD){
	Transmitter* tmt = new Transmitter( clientFD, this->logger );
	tmt->sendData( this->gameData );
	delete tmt;
}

void Server::sendGameDataAll(){
	for( map<int, Player*>::iterator it = this->players.begin(); it != this->players.end(); ++it ) {
		this->sendGameData(it->first);
	}
}

void Server::createGameData(){
  this->gameData = new GameData();
  this->gameData->cooperativeMode = false;
  this->gameData->countPlayersTeam1 = 0;
  this->gameData->countPlayersTeam2 = 0;
  this->gameData->practiceMode = false;
  this->gameData->teamMode = false;
  this->gameData->maxPlayersTeams = this->config->jugadoresPorEquipo;
}

void Server::sendPlayersReady(){
  bool playerReady = true;
  mutex theMutex;
  theMutex.lock();
  for( map<int, Player*>::iterator it = this->players.begin(); it != this->players.end();  ++it ) {
	// if game is running and player is inactive, skip
	if( this->running && !( it->second->isActive() ) ) continue;
	// if already a player with that color
	if( !it->second->isReady()){
		playerReady = false;
//		cout<<"player no ready"<<it->second->getName()<<endl;
	}
  }
  theMutex.unlock();
  if ( playerReady ){
	  cout<<"Iniciando la Partida"<<endl;
	  cout<<"Equipos Configurados  "<<endl;
	// send other players data
	for (map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end(); ++it) {
	  cout <<"Jugador: "<<it->second->getName()<<" - Equipo: "<<it->second->getTeam()<<endl;
	  Transmitter* tmt = new Transmitter(it->first, this->logger);
	  tmt->sendDataID("OK");
	  delete tmt;
	}
  }
}

void Server::setTeamPlayer(int team, int cliendFd){
 for( map<int, Player*>::iterator it = this->players.begin(); it != this->players.end();  ++it ) {
	 if (it->first == cliendFd){
//		 cout <<"Set team: "<<team<<endl;
		 it->second->setTeam(team);
	 }
 }
}

void Server::freePlayerSlot( int clientFD ) {
  string playerName;
  map<int, Player*>::iterator it = this->players.find( clientFD );

  // if player exists
  if ( it != this->players.end() ) {
    playerName = this->players[ clientFD ]->getName();
    // delete player
    delete this->players[ clientFD ];
    // free player slot in hash
    this->players.erase( it );
  }

  // if game is running
  if ( this->running ) {
    Evento* ev = new Evento;
    ev->value = QUITGAME;
    strcpy( ev->name, playerName.c_str() );
    for ( map<int, Player*>::iterator it = this->players.begin();
	  it != this->players.end();
	  ++it ) {
      Transmitter* tmt = new Transmitter( it->first, this->logger );
      tmt->sendData( ev );
      delete tmt;
    }

    delete ev;

    // if playing on team mode, check if remaining players win
    if ( this->gameData->teamMode ) {
      this->checkTeamWin();
    }
  }
}

void Server::checkTeamWin() {
  int teamAlpha = 0, teamBeta = 0;
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Player* player = it->second;
    // if player is alive, check player team
    if ( player->isAlive() ) {
      if ( player->getTeam() == 1 ) { 
	teamAlpha++; 
      } else if ( player->getTeam() == 2 ) {
	teamBeta++; 
      }
    }
  } 

  if ( teamAlpha == 0 ) {
    this->sendTeamWin( "BW" ); 
  } else if ( teamBeta == 0 ) {
    this->sendTeamWin( "AW" ); 
  }
}

void Server::checkCoopLose() {
  int alivePlayersCount = 0; 
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Player* player = it->second;
    // check if player is alive
    if ( player->isAlive() ) {
      alivePlayersCount++; 
    }
  }

  if ( alivePlayersCount == 0 ) {
    this->sendCoopLose();
  }
}

void Server::sendCoopLose() {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Transmitter* tmt = new Transmitter( it->first, this->logger );
    tmt->sendDataID( "CL" ); 
    delete tmt;
  } 
}

void Server::sendTeamWin( string winningTeam ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Transmitter* tmt = new Transmitter( it->first, this->logger );
    tmt->sendDataID( winningTeam.c_str() ); 
    delete tmt;
  } 
}
