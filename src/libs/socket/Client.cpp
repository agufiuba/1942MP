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

Client::Client( const char* configFileName ) {
  this->socketFD = 0;
  this->connected = false;
  this->logger = Logger::instance();
  this->configComplete = false;
  this->reset = false;
  this->config = new GameConf;
  this->stageOffset = 0;
}

Client::Client( string ip, string puerto ) {
  this->socketFD = 0;
  this->connected = false;
  this->logger = Logger::instance();
  this->ip = ip;
  this->puerto = puerto;
  this->configComplete = false;
  this->reset = false;
  this->config = new GameConf;
  this->stageOffset = 0;
}

Client::Client( string ip, string puerto ,HandlerPlayersControllers* handlerPlayersControllers) {
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
}

Client::~Client() {}

void Client::setHandler(HandlerPlayersControllers* handlerPlayersControllers){
  this->pc = handlerPlayersControllers;
}

bool Client::allPlayersReady(){
  if (this->configComplete){
    return (allPlayers.size() == this->config->maxClients);
  }
  return false;
}

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

    return true;
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
    if (setsockopt(this->socketFD, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
	  sizeof(timeout)) < 0) {
      cout << "Error sockopt" << endl;
      exit(1);
    }

    // Get id of next data to receive
    bytesReceived = tmt->receiveData(id, sizeof(id));

    if( bytesReceived > 0 ) {
      string dataID(id);
      // Receive data type based on fetched dataID
      if (dataID == "PD") {
	PlayerData* data = new PlayerData;

	if ((bytesReceived = tmt->receiveData(data)) > 0) {
	  // Process received data
	  cout << "Nombre del jugador: " << string(data->name) << endl;
	  cout << "Color del jugador: " << string(data->color) << endl;
	  string name(data->name);
	  string color(data->color);
	  bool newPlayer = ( name == "Y" && color == "Y" );
	  bool resumePlayer = ( name == "R" && color == "R" );
	  if ( newPlayer || resumePlayer ) {
	    this->playerOk = true;
	  } else {
	    this->playerOk = false;
	  }
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
	  if (e->value == 'T'){
	    this->reset =true;
	  }else {
	    this->pc->mover(e->name, e->value);
	  }

	}
      } else if (dataID == "PR") {
	PlayerData* data = new PlayerData;
	if ((bytesReceived = tmt->receiveData(data)) > 0) {
	  // Process received data
	  cout << "READY -->Nombre del jugador: " << string(data->name) << endl;
	  cout << "READY -->Color del jugador: " << string(data->color) << endl;
	  cout << "READY -->Posicion X del jugador: " << data->x << endl;
	  cout << "READY -->Posicion Y del jugador: " << data->y << endl;
	  cout << "READY -->Equipo del jugador: " << data->team << endl;
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
	  //					cout<<cant<<endl;
	  this->config->maxClients = cant;
	  this->config->elementos = this->elementos;
	  this->config->sprites = this->sprites;
	  this->configComplete = true;
	}
      } else if ( dataID == "SQ" ) {
	this->sendStageData();
      } else if ( dataID == "SD" ) {
	StageData* data = new StageData;
	if ((bytesReceived = tmt->receiveData( data )) > 0) {
	  cout << "Current stage offset: " << data->offset << endl;
	  this->stageOffset = data->offset; 
	}
      } else if ( dataID == "SP" ) {
	PlayerScore* data = new PlayerScore;
	if ((bytesReceived = tmt->receiveData( data )) > 0 ) {
	  this->playersScoreData.push_back( data );
	}
      }
    }

    // Check peer disconnection or timeout
    if ( bytesReceived <= 0 ) {
      this->connected = false;
      close(this->socketFD);
      if( bytesReceived == 0 ) {
	this->logger->warn( CONNECTION_LOST);
	DEBUG_WARN(CONNECTION_LOST);
      } else {
	this->logger->warn( CONNECTION_TIMEOUT );
	DEBUG_WARN( CONNECTION_TIMEOUT );
      }
    }
  }

  delete tmt;
}

bool Client::sendData( Evento* e ) {
  this->received = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  return tmt->sendData( e );
}

bool Client::sendData( PlayerData* data ) {
  this->received = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  return tmt->sendData( data );
}

bool Client::sendScore( PlayerScore* data ) {
  this->received = false;
  bool sent;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  sent = tmt->sendData( data );

  delete tmt;
  return sent;
}

bool Client::sendDataPosicion( PlayerData* data) {
  this->received = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  return tmt->sendDataDisconnect(data);
}

bool Client::sendStageData() {
  this->received = false;
  bool sent;
  StageData* data = new StageData;
  data->offset = this->stageOffset;

  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  sent = tmt->sendData( data, "SD" );

  delete tmt;
  delete data;

  return sent;
}

bool Client::sendPlayerDisconnection() {
  this->received = false;
  bool recvStatus;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  PlayerStatus* data = new PlayerStatus;
  data->status = 'D';
  // Send player status to server
  recvStatus = tmt->sendData( data );

  delete tmt;
  delete data;

  return recvStatus;
}

void Client::disconnectFromServer() {
  mutex theMutex;
  if( this->connected ) {
    this->closeConnection();
    cout<<"close connection"<<endl;
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

PlanesActives* Client::getPlanesActives(){
  return this->planes;
}

bool Client::isPlayerOk(){
  return this->playerOk;
}

vector<PlayerData*> Client::getPlayers() {
  return this->allPlayers;
}

vector<PlayerScore*> Client::getPlayersScoreData() {
  return this->playersScoreData;
}

GameConf* Client::getConfig(){
  return this->config;
}

bool Client::isConfigComplete(){
  return this->configComplete;
}

void Client::setConfigComplete(bool config){
  this->configComplete = config;
}

bool Client::sendGetConfig(){
  this->received = false;
  Transmitter* tmt = new Transmitter( this->socketFD, this->logger );
  return tmt->sendGetConfig();
}

void Client::resetConfig(){
  //delete this->config;
  this->config = new GameConf;
  while(this->elementos.size() > 0){
    this->elementos.pop_back();
    //cout<<"elimino elemento"<<endl;
  }
  while(this->sprites.size() > 0){
    this->sprites.pop_back();
    //cout<<"elimino sprite"<<endl;
  }
}

void Client::setStageOffset( int offset ) {
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
