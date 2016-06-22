#include "Client.h"
#include "../transmitter/Transmitter.h"
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

Client::Client(const char* configFileName) {
	this->socketFD = 0;
	this->connected = false;
	this->logger = Logger::instance();
	this->configComplete = false;
	this->reset = false;
	this->config = new GameConf;
	this->stageOffset = 0;
	this->clientsPlaying = 0;
	this->stageClearReady = false;
	this->player = NULL;
    this->gameData = new GameData;
    this->ready = false;
    this->playerResume = false;
    this->coopTeamScore = 0;
    this->alphaTeamScore = 0;
    this->betaTeamScore = 0;
    this->winner = false;
    this->loser = false;
    this->crearPowerUpGrande = false;
    this->powerUpGrande = new Posicion(0, 0);
}

Client::Client(string ip, string puerto) {
	this->socketFD = 0;
	this->connected = false;
	this->logger = Logger::instance();
	this->ip = ip;
	this->puerto = puerto;
	this->configComplete = false;
	this->reset = false;
	this->config = new GameConf;
	this->stageOffset = 0;
	this->clientsPlaying = 0;
	this->stageClearReady = false;
	this->player = NULL;
    this->gameData = new GameData;
    this->ready = false;
    this->playerResume = false;
    this->coopTeamScore = 0;
    this->alphaTeamScore = 0;
    this->betaTeamScore = 0;
    this->winner = false;
    this->loser = false;
    this->crearPowerUpGrande = false;
    this->powerUpGrande = new Posicion(0, 0);
}

Client::Client(string ip, string puerto,
		HandlerPlayersControllers* handlerPlayersControllers) {
	this->pc = handlerPlayersControllers;
	this->socketFD = 0;
	this->connected = false;
	this->logger = Logger::instance();
	this->ip = ip;
	this->puerto = puerto;
	this->configComplete = false;
	this->reset = false;
	this->config = new GameConf;
	this->stageOffset = 0;
	this->clientsPlaying = 0;
	this->stageClearReady = false;
	this->player = NULL;
    this->gameData = new GameData;
    this->ready = false;
    this->playerResume = false;
    this->coopTeamScore = 0;
    this->alphaTeamScore = 0;
    this->betaTeamScore = 0;
    this->winner = false;
    this->loser = false;
    this->crearPowerUpGrande = false;
    this->powerUpGrande = new Posicion(0, 0);
}

Client::~Client() {
}

void Client::setHandler(HandlerPlayersControllers* handlerPlayersControllers) {
	this->pc = handlerPlayersControllers;
}

void Client::setPowerUpHandler(HandlerPowerUp* hPowerUp) {
	this->hPowerUp = hPowerUp;
}

void Client::setEnemyHandler(HandlerEnemigos* hEnemigos) {
	this->hEnemigos = hEnemigos;
}


bool Client::allPlayersReady(){
  return (this->configComplete && this->ready);
}

bool Client::connectToServer() {
	mutex theMutex;

	if (this->connected) {
		this->logger->warn( CONNECTION_ACTIVE);
		theMutex.lock();
		DEBUG_WARN(CONNECTION_ACTIVE);
		theMutex.unlock();
		return false;
	}

	const int MAX_DATA_SIZE = 10000; /* Max. number of bytes for recv */
	int numBytesRead; /* socketFD, bytes read count */
	char buf[MAX_DATA_SIZE]; /* Received text buffer  */
	struct sockaddr_in server; /* Server address info */

	/* Create socket */
	if ((this->socketFD = socket( AF_INET, SOCK_STREAM, 0)) == -1) {
		this->logger->error( SOCKET_ERROR);
		//exit( -1 );
		return false;
	}

	server.sin_family = AF_INET;

	server.sin_port = htons(stoi(this->puerto));
	if ((inet_aton(this->ip.c_str(), &server.sin_addr)) == 0) {
		this->logger->error("IP invalido");
		return false;
		//exit( -1 );
	}

	bzero(&(server.sin_zero), 8);

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
	if (setsockopt(this->socketFD, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
			sizeof(timeout)) < 0) {
		cout << "Error sockopt" << endl;
		exit(1);
	}

	// Get server welcome message
	if ((numBytesRead = recv(this->socketFD, buf, MAX_DATA_SIZE, 0)) == -1) {
		this->logger->warn( CONNECTION_REJECTED);
		theMutex.lock();
		DEBUG_WARN(CONNECTION_REJECTED);
		theMutex.unlock();
		this->connected = false;
		close(this->socketFD);
	}
	if (numBytesRead > 0) {
		this->logger->info(CONNECTION_SUCCESS(this->ip));
		theMutex.lock();
		DEBUG_PRINT(CONNECTION_SUCCESS( this->ip ));
		buf[numBytesRead] = '\0';
		string recvMsg = string(buf);
		this->logger->info(SERVER_MSG(recvMsg));
		DEBUG_PRINT(SERVER_MSG( recvMsg ));
		theMutex.unlock();
	}
	if (numBytesRead == 0) {
		this->logger->warn( CONNECTION_LOST);
		theMutex.lock();
		DEBUG_WARN(CONNECTION_LOST);
		theMutex.unlock();
		this->connected = false;
		close(this->socketFD);
	}

	// Create thread for receiving data from server
	if (this->connected) {
		thread tCheckAliveSend(&Client::checkAliveSend, this);
		tCheckAliveSend.detach();

		thread tReceiving(&Client::receiving, this, MAX_DATA_SIZE,
				this->ip.c_str());
		tReceiving.detach();

		return true;
	}
}

void Client::checkAliveSend() {
	char buf[1] = { '1' };

	while (true) {
		if (!(this->connected))
			return;
		// 4s timed send
		usleep(4000000);
		send(this->socketFD, &buf, 1, 0);
	}
}

void Client::receiving(const int MAX_DATA_SIZE, const char *IP) {
	timeval timeout;
	timeout.tv_sec = this->MAX_UNREACHABLE_TIME;
	timeout.tv_usec = 0;
	char id[3];
	int bytesReceived;

	// Create transmitter
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);

	while (this->connected) {
		// seteo el timeout de recepcion de mensajes
		if (setsockopt(this->socketFD, SOL_SOCKET, SO_RCVTIMEO,
				(char*) &timeout, sizeof(timeout)) < 0) {
			cout << "Error sockopt" << endl;
			exit(1);
		}

		// Get id of next data to receive
		bytesReceived = tmt->receiveData(id, sizeof(id));

		if (bytesReceived > 0) {
			string dataID(id);
			// Receive data type based on fetched dataID
			if (dataID == "PD") {
				PlayerData* data = new PlayerData;

				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					// Process received data
					cout << "Nombre del jugador: " << string(data->name)
							<< endl;
					cout << "Color del jugador: " << string(data->color)
							<< endl;
					string name(data->name);
					string color(data->color);
					bool newPlayer = (name == "Y" && color == "Y");
					bool resumePlayer = (name == "R" && color == "R");
					this->playerOk = (newPlayer || resumePlayer);
					this->playerResume = resumePlayer;
				}
				delete data;

			} else if (dataID == "PA") {

				PlanesActives* data = new PlanesActives;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->planes = data;
				}

			} else if (dataID == "EV") {
				Evento* e = new Evento();

				if ((bytesReceived = tmt->receiveData(e)) > 0) {
					cout << "Evento: " << e->value << endl;
					cout << "PlayerName: " << e->name << endl;
					if (e->value == 'T') {
						  this->reset = true;
					} else if (e->value == 'Y') {
							this->hPowerUp->matar(string(e->name));
					} else if (e->value == 'X') {
							this->hEnemigos->deleteEnemys();
					} else if (e->value == 'V') {
							this->hEnemigos->clearEnemies();
					} else if (e->value == QUITGAME ) {
					   cout << "QUITGAME TRIGGERED BY: " << string(e->name) << endl; 
					   this->pc->killPlayer( string( e->name ) );
					} else {
							this->pc->mover(e->name, e->value);
					}
				}
			} else if (dataID == "PR") {
				PlayerData* data = new PlayerData;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					// Process received data
					cout << "READY -->Nombre del jugador: "
							<< string(data->name) << endl;
					cout << "READY -->Color del jugador: "
							<< string(data->color) << endl;
					cout << "READY -->Posicion X del jugador: " << data->x
							<< endl;
					cout << "READY -->Posicion Y del jugador: " << data->y
							<< endl;
					cout << "READY -->Equipo del jugador: " << data->team
							<< endl;
					cout << "READY -->Puntaje del jugador: " << data->score
							<< endl;
					this->allPlayers.push_back(data);
				}
			} else if (dataID == "AV") {
				AvionConf* data = new AvionConf;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->config->avion = data;
					//					cout<<data->avionSpriteID<<endl;
					//					cout<<data->disparosSpriteID<<endl;
					//					cout<<data->velocidadDesplazamiento<<endl;
				}
			} else if (dataID == "EL") {
				ElementoConf* data = new ElementoConf;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->elementos.push_back(data);
					//					cout<<data->spriteID <<endl;
					//					cout<<data->x <<endl;
					//					cout<<data->y <<endl;
				}
			} else if (dataID == "PU") {
				PowerUpData* data = new PowerUpData;
				mutex m;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					cout << "POWER UP ID: " << to_string( data->id )<< endl;
					cout << "POWER UP TYPE: " << data->type << endl;
					cout << "POWER UP X: " << to_string( data->x ) << endl;
					cout << "POWER UP Y: " << to_string( data->y ) << endl;
					cout << "POWER UP OFFSET: " << to_string( data->offset ) << endl;
					cout << "POWER UP STATUS: " << data->status << endl;
					m.lock(); 
					this->powerUps.push_back(data);
					m.unlock();
					//					cout<<data->tipo <<endl;
					//					cout<<data->x <<endl;
					//					cout<<data->y <<endl;
				}
			} else if (dataID == "EN") {
				EnemigoConf* data = new EnemigoConf;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->enemigos.push_back(data);
					//					cout<<data->tipo <<endl;
					//					cout<<data->x <<endl;
					//					cout<<data->y <<endl;
				}
			} else if (dataID == "ES") {
				EscenarioConf* data = new EscenarioConf;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->config->escenario = data;
					//					cout<<data->alto <<endl;
					//					cout<<data->ancho <<endl;
					//					cout<<data->fondo <<endl;
				}
			} else if (dataID == "SC") {
				SpriteConf* data = new SpriteConf;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->sprites.push_back(data);
					//					cout<<data->path <<endl;
					//					cout<<data->id <<endl;
					//					cout<<data->alto <<endl;
					//					cout<<data->ancho <<endl;
				}
			} else if (dataID == "FN") {
				char data[1];
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					int cant = atoi(data);
					this->config->maxClients = cant;
					this->config->elementos = this->elementos;
					/*this->config->powerUps = this->powerUps;*/
					this->config->enemigos = this->enemigos;
					this->config->sprites = this->sprites;
					this->configComplete = true;
				}
			} else if (dataID == "SQ") {
				this->sendStageData();
			} else if (dataID == "SD") {
				StageData* data = new StageData;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					cout << "Current stage offset: " << data->offset << endl;
					this->stageOffset = data->offset;
				}
			} else if (dataID == "SP") {
				PlayerScore* data = new PlayerScore;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->playersScoreData.push_back(data);
				}
			} else if (dataID == "PQ") {
				ActivePlayers* data = new ActivePlayers;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
					this->clientsPlaying = data->playerCount;
				}
				delete data;
			} else if ( dataID == "RR" ) {
			  this->stageClearReady = true;
			} else if ( dataID == "AW" ) {
			  if ( this->player->getTeam() == 1 ) {
			    this->winner = true;
			  } else {
			    this->loser = true;
			  }
			} else if ( dataID == "BW" ) {
			  if ( this->player->getTeam() == 2 ) {
			    this->winner = true;
			  } else {
			    this->loser = true;
			  }
			} else if ( dataID == "CL" ) {
			  this->loser = true;
			} else if ( dataID == "TS" ) {
				PlayerScore* data = new PlayerScore;
				mutex m;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
				  // add score to corresponding team
				  m.lock();
				  if ( data->team == 0 ) this->coopTeamScore += data->score;
				  if ( data->team == 1 ) this->alphaTeamScore += data->score;
				  if ( data->team == 2 ) this->betaTeamScore += data->score;
				  m.unlock();
				}
				delete data;
			} else if ( dataID == "SA" ) {
				PlayerScore* data = new PlayerScore;
				if ((bytesReceived = tmt->receiveData(data)) > 0) {
				  this->player->addScore( data->score );
				}
				delete data;
			} else if ( dataID == "GD" ) {
			    GameData* data = new GameData;
			    if ((bytesReceived = tmt->receiveData( data )) > 0 ) {
				  this->gameData = data;
				  if (this->gameData->teamMode){
					  cout<<"Modo de Juego: EQUIPOS"<<endl;
				  } else  if (this->gameData->cooperativeMode){
					  cout<<"Modo de Juego: COOPERATIVO"<<endl;
				  } else {
					  cout<<"Modo de Juego: NO HAY MODO"<<endl;
				  }
			    }
			 } else if ( dataID == "OK" ){
//			   cout<<"Llega el OK"<<endl;
			   this->ready = true;
			 } else if ( dataID == "ED" ) {
				 	EnemyData* data = new EnemyData;
					if ((bytesReceived = tmt->receiveData( data )) > 0 ) {
					  /*cout << "ID: " << to_string( data->id ) << endl;
					  cout << "DIRECTION: " << data->direction << endl;*/
					  Posicion* p = this->hEnemigos->mover(data->id, data->direction);
					  this->hEnemigos->setAvionApuntar( data->id, string( data->playerID ) );
					  if ( p != NULL ) {
					    EnemyStatus* es = new EnemyStatus;
					    es->id = data->id;
					    es->x = p->getX();
					    es->y = p->getY();
					    es->status = 'P';
					    // send new enemy position to Server
					    this->sendEnemyMovements( es );
					    delete es;
					  }
					}
					delete data;
			 } else if ( dataID == "SE" ) {
					EnemyStatus* data = new EnemyStatus;
					if ((bytesReceived = tmt->receiveData( data )) > 0 ) {
					  mutex m;
					  // process enemy status
					  /*cout << "ENEMY ID: " << to_string( data->id ) << endl;
					  cout << "ENEMY TYPE: " << data->type<< endl;
					  cout << "ENEMY POS X: " << to_string( data->x ) << endl;
					  cout << "ENEMY POS Y: " << to_string( data->y ) << endl;
					  cout << "ENEMY OFFSET: " << to_string( data->offset ) << endl;*/
					  cout << "ENEMY STATUS: " << data->status << endl;
					  if ( data->status == 'C' ) {
					    m.lock();
					    this->enemys.push_back( data );
					    m.unlock();
					  } else if ( data->status == 'H' ) {
					    m.lock();
					    this->hEnemigos->bajarHP( data->id ); 
					    m.unlock();
					  } else if ( data->status == 'D' ) {
					    m.lock();
					    this->hEnemigos->matar( data->id );
					    Enemy* e = this->hEnemigos->getEnemigo(data->id);
					    if (e->tipo == 'g') {
					    	this->crearPowerUpGrande = true;
					    	this->powerUpGrande = new Posicion(e->getPosicion()->getX(), e->getPosicion()->getY());
					    }
					    m.unlock();
					  }
					}
			 }
		}

		// Check peer disconnection or timeout
		if (bytesReceived <= 0) {
			this->connected = false;
			close(this->socketFD);
			if (bytesReceived == 0) {
				this->logger->warn( CONNECTION_LOST);
				DEBUG_WARN(CONNECTION_LOST);
			} else {
				this->logger->warn( CONNECTION_TIMEOUT);
				DEBUG_WARN(CONNECTION_TIMEOUT);
			}
		}
	}
	delete tmt;
}

bool Client::sendData(Evento* e) {
	this->received = false;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	return tmt->sendData(e);
}

bool Client::sendData(PlayerData* data) {
	this->received = false;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	return tmt->sendData(data);
}

bool Client::sendScore( PlayerScore* data ) {
	this->received = false;
	bool sent;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	sent = tmt->sendData( data );

	delete tmt;
	return sent;
}

bool Client::sendDataPosicion(PlayerData* data) {
	this->received = false;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	return tmt->sendDataDisconnect(data);
}

bool Client::sendStageData() {
	this->received = false;
	bool sent;
	StageData* data = new StageData;
	data->offset = this->stageOffset;

	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	sent = tmt->sendData(data, "SD");

	delete tmt;
	delete data;

	return sent;
}

int Client::getClientsPlaying() {
	return this->clientsPlaying;
}

void Client::resetClientsPlaying() {
	this->clientsPlaying = 0;
}

void Client::requestScoreTable() {
	this->received = false;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	// request score table
	tmt->sendDataID( "ST" );
	// request clients playing
	tmt->sendDataID( "PQ" );
	delete tmt;
}

bool Client::sendPlayerDisconnection() {
	this->received = false;
	bool recvStatus;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	PlayerStatus* data = new PlayerStatus;
	data->status = 'I';
	// Send player status to server
	recvStatus = tmt->sendData(data);

	delete tmt;
	delete data;

	return recvStatus;
}

bool Client::sendPlayerDeath() {
	this->received = false;
	bool recvStatus;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	PlayerStatus* data = new PlayerStatus;
	data->status = 'D';
	// Send player status to server
	recvStatus = tmt->sendData(data);

	delete tmt;
	delete data;

	return recvStatus;
}

void Client::disconnectFromServer() {
	mutex theMutex;
	if (this->connected) {
		this->closeConnection();
		cout << "close connection" << endl;
	} else {
		this->logger->warn( CONNECTION_NOT_ACTIVE);
		theMutex.lock();
		DEBUG_WARN(CONNECTION_NOT_ACTIVE);
		theMutex.unlock();
	}
}

void Client::shutdownConnection() {
	mutex theMutex;
	this->disconnectFromServer();
	this->logger->warn( CLIENT_CLOSE);
	theMutex.lock();
	DEBUG_WARN(CLIENT_CLOSE);
	theMutex.unlock();
	exit(0);
}

void Client::closeConnection() {
	mutex theMutex;
	close(this->socketFD);
	this->connected = false;
	this->logger->warn( CONNECTION_CLOSE);
	theMutex.lock();
	DEBUG_WARN(CONNECTION_CLOSE);
	theMutex.unlock();
}

PlanesActives* Client::getPlanesActives() {
	return this->planes;
}

bool Client::isPlayerOk() {
	return this->playerOk;
}

vector<PlayerData*> Client::getPlayers() {
	return this->allPlayers;
}

vector<PlayerScore*> Client::getPlayersScoreData() {
	return this->playersScoreData;
}

GameConf* Client::getConfig() {
	return this->config;
}

bool Client::isConfigComplete() {
	return this->configComplete;
}

void Client::setConfigComplete(bool config) {
	this->configComplete = config;
}

bool Client::sendGetConfig() {
	this->received = false;
	Transmitter* tmt = new Transmitter(this->socketFD, this->logger);
	return tmt->sendGetConfig();
}

void Client::resetConfig() {
	//delete this->config;
	this->config = new GameConf;
	while (this->elementos.size() > 0) {
		this->elementos.pop_back();
		//cout<<"elimino elemento"<<endl;
	}
	while (this->sprites.size() > 0) {
		this->sprites.pop_back();
		//cout<<"elimino sprite"<<endl;
	}
	/*while (this->powerUps.size() > 0) {
		this->powerUps.pop_back();
		//cout<<"elimino power up"<<endl;
	}*/
	while (this->enemigos.size() > 0) {
		this->enemigos.pop_back();
	}
}

void Client::setStageOffset(int offset) {
	this->stageOffset = offset;
}

int Client::getStageOffset() {
	return this->stageOffset;
}

bool Client::isConnected() {
	return this->connected;
}

void Client::resetScores() {
	this->playersScoreData.clear();
}

void Client::sendStageClearReady() {
  this->stageClearReady = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendDataID( "RR" );
  delete tmt;
}

void Client::addScoreToPlayer( int score ) {
  mutex m;
  m.lock();
  if ( this->player->getTeam() == 0 ) this->coopTeamScore += score;
  if ( this->player->getTeam() == 1 ) this->alphaTeamScore += score;
  if ( this->player->getTeam() == 2 ) this->betaTeamScore += score;
  m.unlock();

  // create player score data
  PlayerScore* ps = new PlayerScore;
  strcpy( ps->name, ( this->player->getName() ).c_str() );
  ps->team = this->player->getTeam();
  ps->score = score;

  // send score data to server
  this->sendScore( ps );
  delete ps;
}

void Client::requestScoreReset() {
  this->player->resetScore();
  this->coopTeamScore = 0;
  this->alphaTeamScore = 0;
  this->betaTeamScore = 0;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendDataID( "RS" );
  delete tmt;
}

void Client::setPlayer( Player* player ) {
  this->player = player;
}

void Client::requestPlayerScore() {
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendDataID( "GS" ); 
  delete tmt;
}

GameData* Client::getGameData(){
  return this->gameData;
}

void Client::sendGameData(){
  this->received = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendData(this->gameData);
  delete tmt;
}

void Client::sendMode(string mode ){
  this->received = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendDataID(mode);
  delete tmt;
}

void Client::sendStatusReady(){
	  this->received = false;
	  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
	  PlayerStatus* data = new PlayerStatus;
	  data->status = 'R';
	  cout<<"Send player status to server"<<endl;
	  tmt->sendData( data );

	  delete tmt;
	  delete data;

}

bool Client::isPlayerResume(){
	return this->playerResume;
}

int Client::getCoopTeamScore() {
  return this->coopTeamScore;
}

int Client::getAlphaTeamScore() {
  return this->alphaTeamScore;
}

int Client::getBetaTeamScore() {
  return this->betaTeamScore;
}

void Client::setCoopTeamScore( int score ) {
  this->coopTeamScore = score;
}

void Client::setAlphaTeamScore( int score ) {
  this->alphaTeamScore = score;
}

void Client::setBetaTeamScore( int score ) {
  this->betaTeamScore = score;
}

void Client::quitGame() {
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendDataID( "QG" );
  delete tmt;

  this->disconnectFromServer();
  exit(0);
}

bool Client::wins() {
  return this->winner;
}

bool Client::losses() {
  return this->loser;
}

void Client::setCoopMode( bool mode ) {
  this->gameData->cooperativeMode = mode;
}

void Client::setTeamMode( bool mode ) {
  this->gameData->teamMode = mode;
}

void Client::sendEnemyDeath( int id ) {
  EnemyStatus* data = new EnemyStatus;
  data->id = id;
  data->status = 'D';
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendData( data );
  delete data;
  delete tmt;
}

void Client::sendRecoverEnemysAlive() {
  EnemyStatus* data = new EnemyStatus;
  data->status = 'R';
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendData( data );
  delete data;
  delete tmt;
}

vector<EnemyStatus*> Client::getEnemys() {
  return this->enemys;
}

vector<PowerUpData*> Client::getPowerUps() {
  return this->powerUps;
}

void Client::resetEnemys() {
  for ( vector<EnemyStatus*>::iterator it = this->enemys.begin();
	it != this->enemys.end();) {
    if ( ( *it )->status == 'R' ) {
      delete *it;
      it = this->enemys.erase( it );
    } else {
      ++it;
    }
  }
}

void Client::resetPowerUps() {
  for ( vector<PowerUpData*>::iterator it = this->powerUps.begin();
	it != this->powerUps.end();) {
    if ( ( *it )->status == 'R' ) {
      delete *it;
      it = this->powerUps.erase( it );
    } else {
      ++it;
    }
  }
}

void Client::removeEnemy( vector<EnemyStatus*>::iterator it ) {
  delete *it;
  this->enemys.erase( it );
}

void Client::requestEnemyMovements( int id ) {
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  EnemyStatus* data = new EnemyStatus;
  data->id = id;
  data->status = 'A';
  tmt->sendData( data );
  delete tmt;
}

void Client::sendEnemyMovements( EnemyStatus* data ) {
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  tmt->sendData( data );
  delete tmt;
}

void Client::sendEnemyHit( int enemyID, string playerID ) {
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  EnemyStatus* data = new EnemyStatus;
  data->id = enemyID;
  strcpy( data->playerID, playerID.c_str() ); 
  data->status = 'H';

  tmt->sendData( data );
  delete tmt;
  delete data;
}

void Client::sendPowerUpHit( int powerUpID ) {
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  PowerUpData* data = new PowerUpData;
  data->id = powerUpID;
  data->status = 'H';

  tmt->sendData( data );
  delete tmt;
  delete data;
}
