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
  this->file = configFileName;
  this->config = GameParser::parse(file);
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
  this->posicionInicialX = 42;
  this->posicionInicialY = 100;
  this->createGameData();
  //  this->config = XMLParser::parseServerConf( configFileName );
  this->alphaTeamScore = 0;
  this->betaTeamScore = 0;
  this->coopTeamScore = 0;
  this->enemyID = 0;
  this->powerUpID = 0;

	this->numeroDeFlota = 1000;

	this->resumePlayer = false;
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

  if( !( this->flotas.empty() ) ) {
    for( map<int, FlotaObserver*>::iterator it2 = this->flotas.begin();
    it2 != this->flotas.end(); ++it2 ) {
      delete it2->second;
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
				encontrePlayer = true;
				this->resumePlayer = true;
				// resume player game
				cout<<"Resume Game"<<endl;
				selectedColor = it->second->getColor();
				cout << "RESUME SCORE: " << it->second->getScore() << endl;
				score = it->second->getScore();
				posicionInicialX = it->second->getX();
				posicionInicialY = it->second->getY();
				estacionamientoX = it->second->getEstacionamientoX();
				estacionamientoY = it->second->getEstacionamientoY();
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
		Player* p;
		if (encontrePlayer) {
			p = new Player(selectedName, selectedColor, posicionInicialX, posicionInicialY, estacionamientoX, estacionamientoY, selectedTeam);
		} else {
			p = new Player(selectedName, selectedColor, posicionInicialX, posicionInicialY, selectedTeam);
		}
		p->addScore( score );
		theMutex.lock();
		this->players[cfd] = p;
		posicionInicialX += 200;
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
		if(this->resumePlayer) {
			this->resumeClientEnemys();
			this->resumeClientPowerUp();
			this->resumePlayer = false;
		}
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
  // reset player starting positions
  this->posicionInicialX = 42;
  this->posicionInicialY = 100;

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
      pd->estacionamientoX = player->getEstacionamientoX();
      pd->estacionamientoY = player->getEstacionamientoY();
      pd->team = player->getTeam();
      pd->score = player->getScore();

      if ( it2->second->getName() == player->getName() ) {
	pd->coopTeamScore = this->coopTeamScore;
	pd->alphaTeamScore = this->alphaTeamScore;
	pd->betaTeamScore = this->betaTeamScore;
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
  this->config = GameParser::parse(file);
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

  /*vector<PowerUpConf*> powerUps = this->config->powerUps;
    for (int var = 0; var < powerUps.size(); ++var) {
    	PowerUpConf* powerUp = powerUps[var];
      if( !( tmt->sendData( powerUp ) ) ) {
        DEBUG_WARN( "No se pude enviar respuesta a cliente. JOB: Server::send powerUps" );
        this->logger->error( "No se pude enviar respuesta a cliente. JOB: Server::send powerUps" );
      }
    }
*/
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
	  this->coopTeamScore = 0;
	  this->alphaTeamScore = 0;
	  this->betaTeamScore = 0;
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
	} else if ( dataID == "SE" ) {
	  EnemyStatus* data = new EnemyStatus;
	  if( ( bytesReceived = tmt->receiveData( data ) ) > 0 ) {
	    cout << "ENEMY ID: " << to_string( data->id ) << endl;
	    cout << "ENEMY STATUS: " << data->status << endl;
	    mutex m;
	    if ( data->status == 'D' ) {
	      if ( data->id == -1 ) {
		m.lock();
		for( map<int, ServerAvionEnemigo*>::iterator it = this->enemys.begin();
		     it != this->enemys.end(); ) {
		  if ( it->second->isActive() ) {
		    PlayerScore* ps = new PlayerScore;
		    strcpy( ps->name, ( this->players[ cfd ]->getName() ).c_str() );
		    ps->team = this->players[ cfd ]->getTeam();
		    ps->score = this->enemys[ it->first ]->getKillScore();
		    this->addScoreToPlayer( ps );
		    delete ps;
		    delete it->second;
		    it = this->enemys.erase( it );
		  } else {
		    ++it;
		  }
		}
		m.unlock();
	      } else {
		m.lock();
		this->removeEnemy( data->id );
		m.unlock();
	      }
	    } else if (data->status == 'R') {
	      this->createEnemys();
	      this->createPowerUps();
	    } else if ( data->status == 'A' ) {
	      m.lock();
	      this->enemys[ data->id ]->activate();  
	      m.unlock();
	    } else if ( data->status == 'P' ) {
	      m.lock();
	      //cout << "NEW POSITION X OF " << to_string( data->id ) << ": " << to_string( data->x ) << endl; 
	      //cout << "NEW POSITION Y OF " << to_string( data->id ) << ": " << to_string( data->y ) << endl; 
	      if ( this->enemys.find( data->id ) != this->enemys.end() )
		this->enemys[ data->id ]->updatePosition( data->x, data->y );
	      m.unlock();
	    } else if ( data->status == 'H' ) {
	      m.lock();
	      if ( this->enemys.find( data->id ) != this->enemys.end() ) {
	      	// Si es una Flota, analiza si la FlotaObserver detecto que era la ultima y si fue el mismo
	      	// cfd el que lo mato
	      	if (this->enemys[ data->id ]->getType() == 'f') {
	      		ServerAvionEnemigoFlota* flota = (ServerAvionEnemigoFlota*)this->enemys[ data->id ];
	      		flota->bajarHP(cfd);
	      		if (this->flotas[ flota->getNumeroDeFlota() ]->ultimaFlotaYtodosPorElMismo()) {
	      			PlayerScore* ps = new PlayerScore;
	      			strcpy( ps->name, ( this->players[ cfd ]->getName() ).c_str() );
	      			ps->team = this->players[ cfd ]->getTeam();
	      			ps->score = 1000;
	      			this->addScoreToPlayer( ps );
	      			delete ps;
	      			//cout << ">> BONUUUUS --> TODOS LOS AVIONES MATADOS POR: "<< cfd << endl;
	      		}
	      	} else {
            if(this->enemys[data->id]->getType() == 'g') {
              ServerAvionEnemyGrande* grande = (ServerAvionEnemyGrande*)this->enemys[data->id];
              grande->bajarHP(cfd);
              if(!grande->aunVive() && grande->mismoJugador) {
                PlayerScore* bonus = new PlayerScore;
                strcpy(bonus->name, (this->players[cfd]->getName()).c_str());
                bonus->team = this->players[cfd]->getTeam();
                bonus->score = 1500;
                this->addScoreToPlayer(bonus);
                cout << "BONUS POR MATAR AL GRANDE SOLO" << endl;
              }
            } else {
              if (this->enemys[data->id]->getType() == 'm') {
                ServerAvionEnemigoMedio* medio = (ServerAvionEnemigoMedio*)this->enemys[data->id];
                medio->bajarHP(cfd);
                if (!medio->aunVive() && medio->mismoJugador) {
                  PlayerScore* bonus = new PlayerScore;
                  strcpy(bonus->name, (this->players[cfd]->getName()).c_str());
                  bonus->team = this->players[cfd]->getTeam();
                  bonus->score = 500;
                  this->addScoreToPlayer(bonus);
                  cout << "BONUS POR MATAR AL MEDIANO SOLO" << endl;
                }
              }
              else {
	      		    this->enemys[ data->id ]->bajarHP();
              }
            }
	      	}
		PlayerScore* ps = new PlayerScore;
		strcpy( ps->name, ( this->players[ cfd ]->getName() ).c_str() );
		ps->team = this->players[ cfd ]->getTeam();
		ps->score = this->enemys[ data->id ]->getHitScore();
		if ( ps->score > 0 ) {
		  this->addScoreToPlayer( ps );
		}
		// if enemy is still alive, send HP reduction 
		if ( this->enemys[ data->id ]->aunVive() ) {
		  this->sendEnemyUpdate( data, cfd );
		// if enemy is dead, send enemy death
		} else {
		  ps->score = this->enemys[ data->id ]->getKillScore();
		  this->addScoreToPlayer( ps );
		  this->removeEnemy( data->id );
		  this->sendEnemyDeath( data->id, cfd );
		}
		delete ps;
	      }
	      m.unlock();
	    }
	  }
	  delete data;
	} else if ( dataID == "PU" ) {
	  PowerUpData* data = new PowerUpData;
	  mutex m;
	  if( ( bytesReceived = tmt->receiveData( data ) ) > 0 ) {
	    if ( data->status == 'H' ) {
	      m.lock();
	      this->activatePowerUp( data, cfd );
	      m.unlock();
	    } else if ( data->status == 'M' ) {
	      m.lock();
	      cout << "NEW POSITION X OF " << to_string( data->id ) << ": " << to_string( data->x ) << endl; 
	      cout << "NEW POSITION Y OF " << to_string( data->id ) << ": " << to_string( data->y ) << endl; 
	      if ( this->powerUps.find( data->id ) != this->powerUps.end() )
		this->powerUps[ data->id ]->updatePosition( data->x, data->y );
	      m.unlock();

	    }
	  }
	  delete data;
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
    this->enemyID = 0;
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
    Transmitter* tmt = new Transmitter( it->first, this->logger );
    if ( it->second->getName() == string( data->name ) ) {
      it->second->addScore( data->score );
      tmt->sendData( data, "SA" );
    } 

    if( it->second->isActive() ) {
      // send team score
      tmt->sendData( data, "TS" ); 
    }
    delete tmt;
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
    // if player is connected
    if ( player->isActive() ) {
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
	thread tCreateEnemys( &Server::createEnemys, this);
	tCreateEnemys.detach();

	thread tCreatePU( &Server::createPowerUps, this);
	tCreatePU.detach();

	thread tMoveEnemy( &Server::makeEnemyMove, this);
	tMoveEnemy.detach();
  }
}

void Server::createEnemys() {
	this->enemyID = 0;
	this->enemys.clear();
	vector<EnemigoConf*> enemigosConf = this->config->enemigos;
	for (int i = 0; i < enemigosConf.size(); i++) {
		EnemigoConf* enemigoConf = enemigosConf[i];

		this->createEnemy(*enemigoConf->tipo, enemigoConf->x, enemigoConf->y, enemigoConf->apareceEn);
	}
}

void Server::createPowerUps() {
  this->powerUpID = 0;
  vector<PowerUpConf*> powerUpsConf = this->config->powerUps;
  for ( vector<PowerUpConf*>::iterator it = powerUpsConf.begin();
	it != powerUpsConf.end();
	++it ) {
    PowerUpConf* puc = *it;
    this->createPowerUp( *puc->tipo, puc->x, puc->y, puc->apareceEn );
  }
}

void Server::createPowerUp( char type, int x, int y, int offset ) {
  this->powerUpID++;
  this->powerUps[ powerUpID ] = new ServerPowerUp( type, new Posicion( x, y ) );
  this->powerUps[ powerUpID ]->setApareceEn(offset);
  PowerUpData* data = new PowerUpData;
  data->id = this->powerUpID;
  data->type = type;
  data->x = x;
  data->y = y;
  data->offset = offset;
  data->status = 'C';
  this->sendPowerUpCreation( data );
}

void Server::sendPowerUpCreation( PowerUpData* data ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    if ( it->second->isActive() ) {
      Transmitter* tmt = new Transmitter( it->first, this->logger );
      tmt->sendData( data );
      delete tmt;
    }
  }
  delete data;
}

void Server::createEnemy( char type, int x, int y, int offset ) {
  this->enemyID++;
  ServerAvionEnemigo* enemy = NULL;
  if (type == 'g'){
	  enemy = new ServerAvionEnemyGrande( this->enemyID, new Posicion(x, y));
	  enemy->setApareceEn(offset);
	  this->enemys[ enemyID ] =  enemy;
	  this->preparingAndSendingEnemyCreation(type, x, y, offset);

  } else if (type == 'f'){
  	this->createFlota(type, x, y, offset);

  } else if ( type == 'r' ) {
	  enemy = new ServerAvionEnemigoRandom( this->enemyID, new Posicion(x, y));
	  enemy->setApareceEn(offset);
	  this->enemys[ enemyID ] =  enemy;
	  this->preparingAndSendingEnemyCreation(type, x, y, offset);
  } else if ( type == 'm' ) {
	  enemy = new ServerAvionEnemigoMedio( this->enemyID, new Posicion(x, y));
	  enemy->setApareceEn(offset);
	  this->enemys[ enemyID ] =  enemy;
	  this->preparingAndSendingEnemyCreation(type, x, y, offset);
  }
}

void Server::resumeClientEnemys() {
  for ( map<int, ServerAvionEnemigo*>::iterator it = enemys.begin();it != enemys.end(); ++it) {
  	EnemyStatus* data = new EnemyStatus;
  	data->id = it->first;
  	data->type = it->second->getType();
		data->x = it->second->getX();
  	data->y = it->second->getY();
  	data->offset = it->second->getApareceEn();
		data->status = 'C';
    strcpy( data->playerID, ( this->shootPlayerID() ).c_str() );
    this->sendEnemyCreation( data );
  }
}

void Server::resumeClientPowerUp() {
  for ( map<int, ServerPowerUp*>::iterator it = powerUps.begin();it != powerUps.end(); ++it) {
    PowerUpData* data = new PowerUpData;
    data->id = it->first;
    data->type = it->second->getType();
    data->x = it->second->getX();
    data->y = it->second->getY();
    data->offset = it->second->getApareceEn();
    data->status = 'C';
    this->sendPowerUpCreation( data );
  }
}

void Server::preparingAndSendingEnemyCreation(char type, int x, int y, int offset) {
	EnemyStatus* data = new EnemyStatus;
  data->id = this->enemyID;
  data->type = type;
  data->x = x;
  data->y = y;
  data->offset = offset;
  data->status = 'C';
  strcpy( data->playerID, ( this->shootPlayerID() ).c_str() );
  this->sendEnemyCreation( data );
}

void Server::createFlota(char type, int x, int y, int offset) {
	FlotaObserver* fo = new FlotaObserver();
	this->flotas[ numeroDeFlota ] = fo;

	for (int ordenAparicionFlota = 0 ; ordenAparicionFlota < 5 ; ordenAparicionFlota++ ){
		ServerAvionEnemigo* enemy = new ServerAvionEnemigoFlota( this->enemyID, new Posicion(x, y), ordenAparicionFlota, this->numeroDeFlota);
	  ((ServerAvionEnemigoFlota*)enemy)->addObserver(fo);
	  enemy->setApareceEn(offset);
	  this->enemys[ enemyID ] = enemy;

	  this->preparingAndSendingEnemyCreation(type, x, y, offset);

		this->enemyID++;
	}

	this->numeroDeFlota++;
}

void Server::sendEnemyCreation( EnemyStatus* data ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    if ( it->second->isActive() ) {
      Transmitter* tmt = new Transmitter( it->first, this->logger );
      tmt->sendData( data );
      delete tmt;
    }
  }
  delete data;
}

void Server::makeEnemyMove() {
  mutex m;
	EnemyData* data;
	map<int, ServerAvionEnemigo*>::iterator it;
	while (this->running) {
			usleep( 1000000 );
			m.lock();
			for ( it = this->enemys.begin(); it != this->enemys.end(); ++it ) {
			  // send movements if enemy is active
			  if ( it->second->isActive() ) {
			    data = it->second->vivir();
			    if ( data->direction != 'Z' ) {
			      // set player to shoot
			      strcpy( data->playerID, ( this->shootPlayerID() ).c_str() );
			      this->sendEnemyData( data );
			    }
			  }
			}
			m.unlock();
	}
}

void Server::sendEnemyData( EnemyData* data ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    if ( it->second->isActive() ) {
      Transmitter* tmt = new Transmitter( it->first, this->logger );
      tmt->sendData( data );
      delete tmt;
    }
  }
  delete data;
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

string Server::shootPlayerID() {
  if ( this->players.size() == 0 ) return "NA";
  if ( this->players.size() == 1 ) {
    return ( this->players.begin() )->second->getName();
  } else {
    srand( time( NULL ) );
    int offset = rand() % ( this->players.size() );
    map<int, Player*>::iterator it = this->players.begin();
    for ( int i = 0; i < offset; i++ ) {
      ++it;
    }
    return it->second->getName();
  }
}

void Server::removeEnemy( int id ) {
  delete this->enemys[ id ];
  this->enemys.erase( this->enemys.find( id ) );
}

void Server::sendEnemyDeath( int id, int clientFD ) {
  EnemyStatus* data = new EnemyStatus;
  data->id = id;
  data->status = 'D';
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Transmitter* tmt = new Transmitter( it->first, this->logger );
    tmt->sendData( data ); 
    delete tmt;
  } 
  delete data;
}

void Server::sendEnemyUpdate( EnemyStatus* data, int clientFD ) {
  for ( map<int, Player*>::iterator it = this->players.begin();
	it != this->players.end();
	++it ) {
    Transmitter* tmt = new Transmitter( it->first, this->logger );
    tmt->sendData( data ); 
    delete tmt;
  } 
}

void Server::activatePowerUp( PowerUpData* data, int clientFD ) {
  if ( this->powerUps.find( data->id ) == this->powerUps.end() ) return;
  switch ( this->powerUps[ data->id ]->getType() ) {
    case 's':
      break;
    case 'b':{
      PlayerScore* ps = new PlayerScore;
      strcpy( ps->name, ( this->players[ clientFD ]->getName() ).c_str() );
      ps->team = this->players[ clientFD ]->getTeam();
      ps->score = 250;
      this->addScoreToPlayer( ps );
      delete ps;
      break;
    }
    case 'w':
      break;
    case 'd':
      break;
  }
  delete this->powerUps[ data->id ];
  this->powerUps.erase( this->powerUps.find( data->id ) );
}
