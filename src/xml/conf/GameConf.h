#ifndef INC_1942MP_GAMECONFIG_H
#define INC_1942MP_GAMECONFIG_H

#include "vector"

#include "SpriteConf.h"
#include "EscenarioConf.h"
#include "AvionConf.h"

using namespace std;

typedef struct GameConf {
    vector<SpriteConf *> sprites;
    EscenarioConf *escenario;
    AvionConf *avion;
} GameConf;

//class GameConf {
//public:
////    unsigned short int ventanaAncho;
////    unsigned short int ventanaAlto;
//    vector<SpriteConf *> sprites;
//    EscenarioConf *escenario;
//    AvionConf* avion;
//};

#endif
