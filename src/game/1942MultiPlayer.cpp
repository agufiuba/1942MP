#include <iostream>
#include <string>

// #include "../xml/parser/GameParser.h"
#include "view/Escenario.h"

string CLIENT = "client";
string SERVER = "server";

using namespace std;


void startupClient() {

	// Agustin
	//GameConf* gc = GameParser::parse("gc.xml");
	//Escenario* escenario = new Escenario(gc->escenario->ancho, gc->escenario->alto);

	SDL_Event* exitEven = new SDL_Event();
	exitEven->key.keysym.sym = SDLK_r;

	while (exitEven->key.keysym.sym == SDLK_r) {

//		Iniciar Pantalla
		Escenario* escenario = new Escenario(800, 600);
//		Escenario* escenario = new Escenario(true);

//		run
		exitEven = escenario->run();

//		 limpiar memoria
		delete escenario;
	}

}

void startupServer() {
}

void stopClient() {

}

void stopServer() {

}

/**
 * Inicio de juego 1942MP
 * @argv[1] --> modo de inicio del programa: client/server
 * @argv[2] --> archivo de configuracion si se inicia en modo client
 */
int main(int argc, char* argv[]) {

	cout<<argc<<endl;
	if (argc > 1) {
		string mode = argv[1];

		if (mode == CLIENT) {
			const char* fileName = argv[2] ? argv[2] : "default-cc.xml";
			startupClient();

		} else if (mode == SERVER) {
			startupServer();

		} else {
			cout<< "Debe seleccionar modo de inicio con parametro: client/server"
					<< endl;
		}
	} else {
		cout << "Debe seleccionar modo de inicio con parametro: client/server"
				<< endl;
	}

	return 0;
}
