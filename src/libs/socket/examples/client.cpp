#include "../Client.h"
#include "../../menu/Menu.h"
using namespace std;
int main( int argc, char* argv[] ) {
  const char* configFileName = argv[1] ? argv[1] : "default-cc.xml";

  Client* client = new Client( configFileName );

  function<void(void)> connectToServer = bind( &Client::connectToServer, client );
  function<void(void)> disconnectFromServer = bind( &Client::disconnectFromServer, client );
  function<void(void)> shutdownConnection = bind( &Client::shutdownConnection, client );

  Menu clientMenu( "Menu de opciones del Cliente v2" );
  clientMenu.addOption( "Conectar", connectToServer );
  clientMenu.addOption( "Desconectar", disconnectFromServer );
  clientMenu.addOption( "Salir", shutdownConnection );

  clientMenu.display();

  delete client;

  return 0;
}
