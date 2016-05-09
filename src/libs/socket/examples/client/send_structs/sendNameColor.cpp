#include "../../../Client.h"
#include <string>
#include <iostream>
using namespace std;

void displayMenu() {
  cout << endl << "---\tMenu de test de envio de structs\t---" << endl << endl;
  cout << "1. Conectar " << endl;
  cout << "2. Enviar playerData" << endl;
  cout << "0. Salir" << endl;
}

int getOption() {
  int option;
  cout << endl << "Ingrese opcion: ";
  cin >> option;
  return option;
}

int main() {
  string name = "sosuke";
  string color = "blue";
  // Fill player data struct with data
  PlayerData* pData = new PlayerData;
  strcpy( pData->name, name.c_str() );
  strcpy( pData->color, color.c_str() );

  Client* client = new Client( "127.0.0.1", "8000" );

  int option;

  while( true ) {
    displayMenu();
    option = getOption();
    if( option == 1 ) {
      client->connectToServer();
    }
    if( option == 2 ) {
      client->sendData( pData );
    }
    if( option == 0 ) {
      client->shutdownConnection();
    }
  }

  delete client;
  delete pData;

  return 0;
}
