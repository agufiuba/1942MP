#include "Menu.h"
#include <iostream>

using namespace std;

void connect() {
  cout << "Connect function trigger" << endl;
}

void disconnect() {
  cout << "Disconnect function trigger" << endl;
}

void cycle() {
  cout << "Cycle function trigger" << endl;
}

void sendMsg(int msg) {
  cout << "ID del mensaje: " << msg << endl;
}

void salir() {
  cout << "Cerrando el cliente..." << endl;
}

int main() {
  Menu menu("Menu de opciones del Cliente");

  menu.addOption("Conectar", connect);
  menu.addOption("Desconectar", disconnect); 
  menu.addOption("Enviar mensaje <id>", sendMsg, 37);
  menu.addOption("Ciclar", cycle);
  menu.addOption("Salir", salir);

  menu.display();

  return 0;
}
