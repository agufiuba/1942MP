#include <iostream>
#include <queue>
#include "../src/models/msg/Msg.h"
#include <string>
using namespace std;

queue<Msg*>* colaDeMensajes;

void iniciarlizarCola() {
	colaDeMensajes = new queue<Msg*>;
}

/*
 * Elimina todos los elementos de la cola y posteriormente la cola.
 */
void eliminarCola() {
	while (!colaDeMensajes->empty()) {
		Msg* mensaje = colaDeMensajes->front();
		colaDeMensajes->pop();

		if (mensaje != NULL) {
			delete mensaje;
		}
	}
	delete colaDeMensajes;
}

bool testCrearCola() {
	iniciarlizarCola();
	bool test = (colaDeMensajes != NULL);
	eliminarCola();
	return test;
}

bool testDeleteColaVacia() {
	iniciarlizarCola();
	eliminarCola();
	return true;
}

bool testDeleteColaNoVacia() {
	iniciarlizarCola();

	Msg* mensaje = new Msg("1",1,"Mensaje");
	colaDeMensajes->push(mensaje);

	eliminarCola();
	return true;
}

/*
 * Prueba generica de cola con Msg
 */
bool testHolaMundo() {

	iniciarlizarCola();
	Msg* hola = new Msg("1",1,"Hola");
	Msg* mundo = new Msg("2",1,"Mundo");

	colaDeMensajes->push(hola);
	colaDeMensajes->push(mundo);

	string resultado = "";

	while (!colaDeMensajes->empty()) {
		Msg* mensaje = colaDeMensajes->front();
		colaDeMensajes->pop();
		resultado += mensaje->getValue() + " ";
	}
	eliminarCola();

	// Imprime Hola Mundo
	cout << resultado << endl;
	return (resultado == "Hola Mundo ");
}

bool testSize() {
	iniciarlizarCola();
	bool cero = colaDeMensajes->size() == 0;

	Msg* mensaje1 = new Msg("1",1,"Mensaje 1");
	Msg* mensaje2 = new Msg("1",1,"Mensaje 2");

	colaDeMensajes->push(mensaje1);
	bool uno = colaDeMensajes->size() == 1;

	colaDeMensajes->push(mensaje2);
	bool dos = colaDeMensajes->size() == 2;

	colaDeMensajes->pop();
	bool unoPosterior = colaDeMensajes->size() == 1;

	eliminarCola();
	return cero && uno && dos && unoPosterior;
}

bool testPushAndPop() {
	iniciarlizarCola();
	bool vacia = colaDeMensajes->empty();

	Msg* mensaje = new Msg("1",1,"Mensaje");
	colaDeMensajes->push(mensaje);
	bool conElementos = !colaDeMensajes->empty();

	colaDeMensajes->pop();
	bool vaciaPosterior = colaDeMensajes->empty();

	eliminarCola();
	return vacia && conElementos && vaciaPosterior;
}

bool testFrontAndback() {
	iniciarlizarCola();

	Msg* A = new Msg("1",1,"A");
	Msg* B = new Msg("1",1,"B");
	Msg* C = new Msg("1",1,"C");

	colaDeMensajes->push(A);
	colaDeMensajes->push(B);
	colaDeMensajes->push(C);

	bool frontIsA = colaDeMensajes->front()->getValue() == "A";
	bool backIsC = colaDeMensajes->back()->getValue() == "C";

	eliminarCola();
	return frontIsA && backIsC;
}

int main () {

	bool init, deleteVacia, deleteConObjetos, holaMundo, size, pushAndpop, frontAndback;

	init = testCrearCola();
	deleteVacia = testDeleteColaVacia();
	deleteConObjetos = testDeleteColaNoVacia();
	holaMundo = testHolaMundo();
	size = testSize();
	pushAndpop = testPushAndPop();
	frontAndback = testFrontAndback();

	bool test = init && deleteVacia && deleteConObjetos && holaMundo && size && pushAndpop && frontAndback;

	if (!test){
		return 1;
	}
	return 0;
}
