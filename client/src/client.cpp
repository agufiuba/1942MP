#include <iostream>

using namespace std;

typedef unsigned short ushort;

void displayMenu();
ushort inputOption();

void displayMenu() {
  cout << "----\tMenu de opciones de Cliente\t----" << endl;
  cout << "  1. Conectar" << endl;
  cout << "  2. Desconectar" << endl;
  cout << "  3. Ciclar" << endl;
  cout << "  4. Salir" << endl << endl;
  cout << " Seleccione una opcion para continuar: ";
}

ushort inputOption(ushort &option) {
  cin >> option;
}

int main(int argc, char* argv[]) {
  
  const char* fileName = argv[1]; 
  
  if(!fileName) fileName = "default-client.xml";

  bool quit = false;
  ushort option = 0;

  while(!quit) {
    displayMenu();
    inputOption(option);

    switch(option) {
      case 1: 
	break;
      case 2:
	break;
      case 3:
	break;
      case 4:
	quit = true;
	cout << endl << "Cerrando cliente..." << endl;
	break;
      default:
	cout << "La opcion ingresada es invalida!" << endl;
    }
  }

  return 0;
}
