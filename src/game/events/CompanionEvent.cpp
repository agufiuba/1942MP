#include "CompanionEvent.h"

Evento CompanionEvent::right(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = DERECHA;
    return e;
}

Evento CompanionEvent::left(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = IZQUIERDA;
    return e;
}

Evento CompanionEvent::up(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = ARRIBA;
    return e;
}

Evento CompanionEvent::down(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = ABAJO;
    return e;
}

Evento CompanionEvent::shot(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = DISPARO;
    return e;
}

Evento CompanionEvent::ready(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = LISTO;
    return e;
}

Evento CompanionEvent::restart(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = REINICIAR;
    return e;
}

Evento CompanionEvent::quit(char playerID) {
    Evento e;
    e.playerID = playerID;
    e.value = SALIR;
    return e;
}
