#include "Menu.h"
#include <string>

int main() {
  Menu menu("Menu de opciones del Cliente");

  menu.addOption("Conectar", "functionConnect");
  menu.addOption("Desconectar", "functionDisconnect");
  menu.display();

  return 0;
}
