#ifndef INC_1942MP_COMPANIONEVENT_H
#define INC_1942MP_COMPANIONEVENT_H

#include "Events.cpp"
#include <string>
#include <cstring>

class CompanionEvent {
public:
    static Evento* right(string n);
    static Evento* left(string n);
    static Evento* up(string n);
    static Evento* down(string n);
    static Evento* shot(string n);
    static Evento* roll(string n);
    static Evento* restart(string n);
    static Evento* ready(string n);
    static Evento* quit(string n);
    static Evento* ametralladora(string n);
    static Evento* destroy(string n);
    static Evento* estaciono(string n);
    static Evento* explosion(string n);
};

#endif

