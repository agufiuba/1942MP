#ifndef INC_1942MP_AVIONCONF_H
#define INC_1942MP_AVIONCONF_H

#include "string"

using namespace std;

typedef struct AvionConf {
    int velocidadDesplazamiento;
    int velocidadDisparos;
    char avionSpriteID[30];
    char vueltaSpriteID[30];
    char disparosSpriteID[30];
} AvionConf;

#endif
