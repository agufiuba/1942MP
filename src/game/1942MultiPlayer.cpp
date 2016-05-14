#include <iostream>
#include <string>

#include "../xml/parser/GameParser.h"
#include "view/Escenario.h"
#include "Game.h"

string CLIENT = "client";
string SERVER = "server";

using namespace std;


void startupClient() {
	Game* game = new Game();
	game->start();
	// Delete game
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
