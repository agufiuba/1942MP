#ifndef INC_1942MP_COMPANIONEVENT_H
#define INC_1942MP_COMPANIONEVENT_H

#include "Events.cpp"

class CompanionEvent {
public:
    static Evento right(char);
    static Evento left(char);
    static Evento up(char);
    static Evento down(char);
    static Evento shot(char);
    static Evento restart(char);
    static Evento ready(char);
    static Evento quit(char);
};

#endif
