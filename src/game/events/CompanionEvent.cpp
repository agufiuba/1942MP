#include "CompanionEvent.h"

Evento* CompanionEvent::right() {
		Evento* e = new Evento();
    e->value = DERECHA;
    return e;
}

Evento* CompanionEvent::left() {
		Evento* e = new Evento();
    e->value = IZQUIERDA;
    return e;
}

Evento* CompanionEvent::up() {
		Evento* e = new Evento();
    e->value = ARRIBA;
    return e;
}

Evento* CompanionEvent::down() {
		Evento* e = new Evento();
    e->value = ABAJO;
    return e;
}

Evento* CompanionEvent::shot() {
		Evento* e = new Evento();
    e->value = DISPARO;
    return e;
}

Evento* CompanionEvent::roll() {
		Evento* e = new Evento();
    e->value = VUELTERETA;
    return e;
}

Evento* CompanionEvent::ready() {
    Evento* e = new Evento();
    e->value = LISTO;
    return e;
}

Evento* CompanionEvent::restart() {
		Evento* e = new Evento();
    e->value = REINICIAR;
    return e;
}

Evento* CompanionEvent::quit() {
		Evento* e = new Evento();
    e->value = SALIR;
    return e;
}
