#ifndef INC_1942MP_GAMECONFIG_H
#define INC_1942MP_GAMECONFIG_H

#include "vector"

#include "SpriteConf.h"
#include "EscenarioConf.h"
#include "AvionConf.h"
#include "ElementoConf.h"

using namespace std;

typedef struct GameConf {
    vector<SpriteConf *> sprites;
    EscenarioConf *escenario;
    AvionConf *avion;
    int maxClients;
    vector<ElementoConf *> elementos;
} GameConf;

#endif
