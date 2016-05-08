#include "Transmitter.h"
#include "../socket/sock_dep.h"
#include "../palette/palette.h"
#define DEBUG 1
#include "../debug/dg_msg.h"
#include <string>
#include <mutex>
#include <iostream>
using namespace std;

Transmitter::Transmitter( int peerFD, Logger* logger ) {
  this->peerFD = peerFD;
  this->logger = logger;
}

Transmitter::~Transmitter() {}

bool Transmitter::sendData( PlayerData* data ) {
  char id[2] = { 'P', 'D' };
  if( send( this->peerFD, id, sizeof( id ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  if( send( this->peerFD, data, sizeof( PlayerData ), 0 ) == -1 ) {
    this->logger->error( SEND_FAIL );
    DEBUG_WARN( SEND_FAIL );
    return false;
  }

  return true;
}

bool Transmitter::receiveData( char id[2], int size ) {
  int numBytesRead;
  // Read data id
  if( ( numBytesRead = recv( this->peerFD, id, size, 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}

bool Transmitter::receiveData( PlayerData* data ) {
  int numBytesRead;
  // Read data
  if( ( numBytesRead = recv( this->peerFD, data, sizeof( PlayerData ), 0 ) ) == -1 ) {
    close( this->peerFD );
    this->logger->warn( CONNECTION_TIMEOUT );
    DEBUG_WARN( CONNECTION_TIMEOUT );
  }

  return ( numBytesRead > 0 );
}
