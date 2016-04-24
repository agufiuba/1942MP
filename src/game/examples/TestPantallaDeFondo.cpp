#include <iostream>
#include "../model/FondoDePantalla.h"

int main() {
	cout << "Empezando test..." << endl;

	FondoDePantalla* fondo = new FondoDePantalla();
	fondo->run();
	fondo->~FondoDePantalla();

	cout << "Finalizando test..." << endl;
	return 0;
}

