#include "../../Server.h"
#include "../../../menu/Menu.h"

using namespace std;

int main(int argc, char* argv[]) {
  const char* configFileName = argv[1] ? argv[1] : "default-sc.xml"; 

  Server* server = new Server( configFileName );
  function<void(void)> initialize = bind( &Server::initialize, server );
  
  Menu serverMenu( "Menu de opciones del Servidor" );

  serverMenu.addOption( "Iniciar servidor", initialize );

  serverMenu.display();

  delete server;

  return 0;
}
