#ifndef INC_1942MP_POWERUPCONF_H
#define INC_1942MP_POWERUPCONF_H

#include "string"
#include "vector"

using namespace std;

typedef struct PowerUpConf {
    char tipo[30];
    int x;
    int y;
    int apareceEn;
} PowerUpConf;

#endif
