#include "CompanionEvent.h"

Evento* CompanionEvent::right(string n) {
		Evento* e = new Evento();
    e->value = DERECHA;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::left(string n) {
		Evento* e = new Evento();
    e->value = IZQUIERDA;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::up(string n) {
		Evento* e = new Evento();
    e->value = ARRIBA;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::down(string n) {
		Evento* e = new Evento();
    e->value = ABAJO;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::shot(string n) {
		Evento* e = new Evento();
    e->value = DISPARO;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::roll(string n) {
		Evento* e = new Evento();
    e->value = VUELTERETA;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::ready(string n) {
    Evento* e = new Evento();
    e->value = LISTO;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::restart(string n) {
		Evento* e = new Evento();
    e->value = REINICIAR;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::quit(string n) {
		Evento* e = new Evento();
    e->value = SALIR;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::ametralladora(string n) {
		Evento* e = new Evento();
    e->value = AMETRALLADORA;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::destroy(string n) {
		Evento* e = new Evento();
    e->value = DESTROY;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::estaciono(string n) {
		Evento* e = new Evento();
    e->value = ESTACIONO;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::explosion(string n) {
		Evento* e = new Evento();
    e->value = EXPLOSION;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::powerUpDestroy(string n) {
		Evento* e = new Evento();
    e->value = POWERUPDESTROY;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::outPractice(string n){
	Evento* e = new Evento();
    e->value = OUTPRACTICE;
    strcpy(e->name, n.c_str());
    return e;
}

Evento* CompanionEvent::clearEnemies(string n){
	Evento* e = new Evento();
    e->value = CLEAR_ENEMIES;
    strcpy(e->name, n.c_str());
    return e;
}
