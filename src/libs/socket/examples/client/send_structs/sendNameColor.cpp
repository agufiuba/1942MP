#include "../../../Client.h"
#include "../../../../../game/events/CompanionEvent.h"

#include <string>
#include <iostream>
using namespace std;

void displayMenu() {
  cout << endl << "---\tMenu de test de envio de structs\t---" << endl << endl;
  cout << "1. Conectar " << endl;
  cout << "2. Enviar playerData => name: sosuke, color: blue" << endl;
  cout << "3. Enviar playerData => name: sosuke, color: yellow" << endl;
  cout << "4. Enviar eventos de avion" << endl;
  cout << "0. Salir" << endl;
}

int getOption() {
  int option;
  cout << endl << "Ingrese opcion: ";
  cin >> option;
  return option;
}

int main() {
  string name1 = "sosuke";
  string color1 = "blue";
  string name2 = "sosuke";
  string color2 = "yellow";
  // Fill player data struct with data
  PlayerData* pData1 = new PlayerData;
  strcpy( pData1->name, name1.c_str() );
  strcpy( pData1->color, color1.c_str() );

  PlayerData* pData2 = new PlayerData;
  strcpy( pData2->name, name2.c_str() );
  strcpy( pData2->color, color2.c_str() );

  Client* client = new Client( "127.0.0.1", "8000" );

  int option;

  while( true ) {
    displayMenu();
    option = getOption();
    if( option == 1 ) {
      client->connectToServer();
    }
    if( option == 2 ) {
      client->sendData( pData1 );
    }
    if( option == 3 ) {
      client->sendData( pData2 );
    }
    if( option == 0 ) {
      client->shutdownConnection();
    }
    if( option == 4 ) {
      Evento* e;
      CompanionEvent* ce = new CompanionEvent();

      e = ce->down();
      client->sendData(e);

      e = ce->up();
      client->sendData(e);

      e = ce->right();
      client->sendData(e);

      e = ce->left();
      client->sendData(e);

      e = ce->shot();
      client->sendData(e);

      e = ce->roll();
      client->sendData(e);

    }
  }

  delete client;
  delete pData1;
  delete pData2;

  return 0;
}
