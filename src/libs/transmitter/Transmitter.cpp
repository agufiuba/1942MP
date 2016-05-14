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

bool Transmitter::receiveData( Evento* e , int & b) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, e, sizeof( Evento ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}

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

bool Transmitter::receiveData( char id[3], int size , int & b) {
  int numBytesRead;
  // Read data id
  if( ( numBytesRead = recv( this->peerFD, id, size, 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}

bool Transmitter::receiveData( PlayerData* data, int & b ) {
  // Read data
  b = recv( this->peerFD, data, sizeof( PlayerData ), 0 ) ;

  return ( b > 0 );
}

bool Transmitter::receiveData( PlayerStatus* data, int & b ) {
  // Read data
  b = recv( this->peerFD, data, sizeof( PlayerStatus ), 0 ) ;

  return ( b > 0 );
}

bool Transmitter::receiveData( PlanesActives* data, int & b ) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, data, sizeof( PlanesActives ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
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



bool Transmitter::receiveData( AvionConf* data ) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, data, sizeof( AvionConf ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}


bool Transmitter::receiveData( ElementoConf* data ) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, data, sizeof( ElementoConf ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}


bool Transmitter::receiveData( EscenarioConf* data ) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, data, sizeof( EscenarioConf ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}


bool Transmitter::receiveData( SpriteConf* data ) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, data, sizeof( SpriteConf ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}


bool Transmitter::sendEndDataConf(){
 // Send data id
  if( !( this->sendDataID( "END" ) ) ) {
	return false;
  }
  return true;
}

