#include "../../Server.h"
#include "../../../menu/Menu.h"

using namespace std;

int main(int argc, char* argv[]) {
  for (int i=0; i < argc; i++) {
	  cout << "Levantando " << argv[i] << endl;
  }
  const char* configFileName = argv[2] ? argv[2] : "gameconf.xml";
  cout << "Cargando archivo " << configFileName << endl;

  Server* server = new Server( configFileName );
  function<void(void)> initialize = bind( &Server::initialize, server );
  function<void(void)> shutdown = bind( &Server::shutdown, server );

  Menu serverMenu( "Menú de opciones del Servidor" );

  serverMenu.addOption( "Iniciar servidor", initialize );
  serverMenu.addOption( "Salir", shutdown );

  serverMenu.display();

  delete server;

  return 0;
}
