#include <iostream>
#include "controler/keyboardController.h"

using namespace std;

void startupClient(){

	//Iniciar Pantalla

	//Inicia controlador de Teclado
	keyboardController controller = new keyboardController();

	//Cargar jugador

}

void startupServer(){

}

void stopClient(){

}

void stopServer(){

}


/**
 * Inicio de juego 1942MP
 * @argv[1] --> modo de inicio del programa: client/server
 * @argv[2] --> archivo de configuracion si se inicia en modo client
 */
int main(int argc, char* argv[]) {

  const char* mode = argv[1];

  //Modo Cliente
  if ( mode == "client" ){

    const char* fileName = argv[2] ? argv[2] : "default-cc.xml";

    startupClient();

  }

  if ( mode == "server" ){

  	startupServer();

  }

  cout << "Seleccione modo de inicio: client/server" << endl;

  return 0;
}
