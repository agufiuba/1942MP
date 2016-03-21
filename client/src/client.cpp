#ifndef MENU_H
#include "../libs/menu/Menu.h"
#endif

void connect(){}
void disconnect(){}
void cycle(){}
void exitPgm(){}

int main(int argc, char* argv[]) {
  
  const char* fileName = argv[1]; 
  if(!fileName) fileName = "default-client.xml";

  Menu clientMenu("Menu de opciones del Cliente"); 

  clientMenu.addOption("Conectar", connect);
  clientMenu.addOption("Desconectar", disconnect);
  clientMenu.addOption("Ciclar", cycle);
  clientMenu.addOption("Salir", exitPgm);

  clientMenu.display();

  return 0;
}
