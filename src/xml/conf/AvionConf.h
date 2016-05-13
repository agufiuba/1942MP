#ifndef INC_1942MP_AVIONCONF_H
#define INC_1942MP_AVIONCONF_H

#include "string"

using namespace std;

typedef struct AvionConf {
    int velocidadDesplazamiento;
    int velocidadDisparos;
    char avionSpriteID[20];
    char vueltaSpriteID[20];
    char disparosSpriteID[20];
} AvionConf;

#endif
