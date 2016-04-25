#include "../Client.h"
#include "../../menu/Menu.h"

using namespace std;

Menu clientMenu( "Menu de opciones del Cliente v2" );

void addMsgOptions(Client* client) {
  for (int i = 0; i < client->getMessages().size(); i++) {
    string msgID = string( client->getMessages()[i]->id );
    string optionName = "Enviar mensaje " + msgID;
    function<bool(string)> sendMsg = bind( &Client::sendMsg, client, msgID );
    clientMenu.addOption( optionName, sendMsg, msgID );
  }
}

int main( int argc, char* argv[] ) {
  const char* configFileName = argv[1] ? argv[1] : "default-cc.xml";

  Client* client = new Client( configFileName );
  function<void(void)> connectToServer = bind( &Client::connectToServer, client );
  function<void(void)> disconnectFromServer = bind( &Client::disconnectFromServer, client );
  function<void(void)> shutdownConnection = bind( &Client::shutdownConnection, client );
  function<void(void)> sendCycle = bind( &Client::sendCycle, client );

  clientMenu.addOption( "Conectar", connectToServer );
  clientMenu.addOption( "Desconectar", disconnectFromServer );
  addMsgOptions(client);
  clientMenu.addOption( "Ciclar", sendCycle );
  clientMenu.addOption( "Salir", shutdownConnection );

  clientMenu.display();

  delete client;

  return 0;
}
