#include <iostream>
#include "../model/FondoDePantalla.h"

int main() {
	cout << "Empezando test..." << endl;

	FondoDePantalla* fondo = new FondoDePantalla();
//	FondoDePantalla* fondo = new FondoDePantalla(800, 600);
//	FondoDePantalla* fondo = new FondoDePantalla(150, 320, 200);
	fondo->run();
	fondo->~FondoDePantalla();

	cout << "Finalizando test..." << endl;
	return 0;
}

