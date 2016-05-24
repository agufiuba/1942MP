#ifndef INC_1942MP_SPRITECONF_H
#define INC_1942MP_SPRITECONF_H

#include <string>
#include <vector>

using namespace std;

typedef struct SpriteConf {
    char id[20];
    char path[30];
    int ancho;
    int alto;
} SpriteConf;

#endif
