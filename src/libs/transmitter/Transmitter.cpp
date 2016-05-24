#include "Transmitter.h"
#include "../socket/sock_dep.h"
#include "../palette/palette.h"
#define DEBUG 1
#include "../debug/dg_msg.h"
#include <mutex>
#include <iostream>

Transmitter::Transmitter( int peerFD, Logger* logger ) {
  this->peerFD = peerFD;
  this->logger = logger;
}

Transmitter::~Transmitter() {}

bool Transmitter::sendDataID( string id ) {
  // Add string end mark
  id += "\0";
  char dataID[3];
  strcpy( dataID, id.c_str() );
  
  if( send( this->peerFD, dataID, sizeof( dataID ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( Evento* e ) {
  // Send data id
  if( !( this->sendDataID( "EV" ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, e, sizeof( Evento ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( PlayerData* data ) {
  // Send data id
  if( !( this->sendDataID( "PD" ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( PlayerData ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( StageData* data, string id ) {
  // Send data id
  if( !( this->sendDataID( id ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( StageData ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendDataDisconnect( PlayerData* data ) {
  // Send data id
  if( !( this->sendDataID( "DP" ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( PlayerData ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( PlayerData* data, string s ) {
  // Send data id
  if( !( this->sendDataID( s ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( PlayerData ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( PlayerStatus* data ) {
  // Send data id
  if( !( this->sendDataID( "PS" ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( PlayerStatus ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( PlanesActives* data ) {
  // Send data id
  if( !( this->sendDataID( "PA" ) ) ) {
    return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( PlanesActives ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::sendData( AvionConf* data ){
 // Send data id
  if( !( this->sendDataID( "AV" ) ) ) {
	return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( AvionConf ), 0 ) == -1 ) {
	this->logger->error( SEND_FAIL );
	DEBUG_WARN( SEND_FAIL );
	return false;
  }

  return true;
}

bool Transmitter::sendData( ElementoConf* data ){
 // Send data id
  if( !( this->sendDataID( "EL" ) ) ) {
	return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( ElementoConf ), 0 ) == -1 ) {
	this->logger->error( SEND_FAIL );
	DEBUG_WARN( SEND_FAIL );
	return false;
  }

  return true;
}

bool Transmitter::sendData( EscenarioConf* data ){
 // Send data id
  if( !( this->sendDataID( "ES" ) ) ) {
	return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( EscenarioConf ), 0 ) == -1 ) {
	this->logger->error( SEND_FAIL );
	DEBUG_WARN( SEND_FAIL );
	return false;
  }

  return true;
}

bool Transmitter::sendData( SpriteConf* data ){
 // Send data id
  if( !( this->sendDataID( "SC" ) ) ) {
	return false;
  }

  // Send data
  if( send( this->peerFD, data, sizeof( SpriteConf ), 0 ) == -1 ) {
	this->logger->error( SEND_FAIL );
	DEBUG_WARN( SEND_FAIL );
	return false;
  }

  return true;
}

bool Transmitter::sendEndDataConf(int cantidad){
 // Send data id
  if( !( this->sendDataID( "FN" ) ) ) {
	return false;
  }

  // Send data
  if( send( this->peerFD, to_string(cantidad).c_str(), 1, 0 ) == -1 ) {
	this->logger->error( SEND_FAIL );
	DEBUG_WARN( SEND_FAIL );
	return false;
  }

  return true;
}

bool Transmitter::sendGetConfig(){
 // Send data id
  //cout<<" Send CO "<<endl;
  if( !( this->sendDataID( "CO" ) ) ) {
	return false;
  }
  return true;
}

int Transmitter::receiveData( char id[3], int size ) {
  return recv( this->peerFD, id, size, 0 );
}

int Transmitter::receiveData( Evento* e ) {
  return recv( this->peerFD, e, sizeof( Evento ), 0 );
}

int Transmitter::receiveData( PlayerData* data ) {
  return recv( this->peerFD, data, sizeof( PlayerData ), 0 ) ;
}

int Transmitter::receiveData( StageData* data ) {
  return recv( this->peerFD, data, sizeof( StageData ), 0 ) ;
}

int Transmitter::receiveData( PlayerStatus* data ) {
  return recv( this->peerFD, data, sizeof( PlayerStatus ), 0 ) ;
}

int Transmitter::receiveData( PlanesActives* data ) {
  return recv( this->peerFD, data, sizeof( PlanesActives ), 0 ) ;
}

int Transmitter::receiveData( AvionConf* data ) {
  return recv( this->peerFD, data, sizeof( AvionConf ), 0 ) ;
}

int Transmitter::receiveData( ElementoConf* data ) {
  return recv( this->peerFD, data, sizeof( ElementoConf ), 0 ) ;
}

int Transmitter::receiveData( EscenarioConf* data ) {
  return recv( this->peerFD, data, sizeof( EscenarioConf ), 0 ) ;
}

int Transmitter::receiveData( SpriteConf* data ) {
  return recv( this->peerFD, data, sizeof( SpriteConf ), 0 ) ;
}

int Transmitter::receiveData( char data[1] ) {
  return recv( this->peerFD, data, 1, 0 ) ;
}
