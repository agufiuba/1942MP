#ifndef INC_1942MP_COMPANIONEVENT_H
#define INC_1942MP_COMPANIONEVENT_H

#include "Events.cpp"

class CompanionEvent {
public:
    static Evento* right();
    static Evento* left();
    static Evento* up();
    static Evento* down();
    static Evento* shot();
    static Evento* roll();
    static Evento* restart();
    static Evento* ready();
    static Evento* quit();
};

#endif
