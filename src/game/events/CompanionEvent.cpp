#include "CompanionEvent.h"

Evento* CompanionEvent::right(string n) {
		Evento* e = new Evento();
    e->value = DERECHA;
    return e;
}

Evento* CompanionEvent::left(string n) {
		Evento* e = new Evento();
    e->value = IZQUIERDA;
    return e;
}

Evento* CompanionEvent::up(string n) {
		Evento* e = new Evento();
    e->value = ARRIBA;
    return e;
}

Evento* CompanionEvent::down(string n) {
		Evento* e = new Evento();
    e->value = ABAJO;
    return e;
}

Evento* CompanionEvent::shot(string n) {
		Evento* e = new Evento();
    e->value = DISPARO;
    return e;
}

Evento* CompanionEvent::roll(string n) {
		Evento* e = new Evento();
    e->value = VUELTERETA;
    return e;
}

Evento* CompanionEvent::ready(string n) {
    Evento* e = new Evento();
    e->value = LISTO;
    return e;
}

Evento* CompanionEvent::restart(string n) {
		Evento* e = new Evento();
    e->value = REINICIAR;
    return e;
}

Evento* CompanionEvent::quit(string n) {
		Evento* e = new Evento();
    e->value = SALIR;
    return e;
}
