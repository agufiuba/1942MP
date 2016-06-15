#ifndef INC_1942MP_GAMECONFIG_H
#define INC_1942MP_GAMECONFIG_H

#include "vector"

#include "SpriteConf.h"
#include "EscenarioConf.h"
#include "AvionConf.h"
#include "ElementoConf.h"
#include "PowerUpConf.h"
#include "EnemigoConf.h"

using namespace std;

typedef struct GameConf {

	int maxClients;
	int jugadoresPorEquipo;
    vector<SpriteConf *> sprites;
    vector<ElementoConf *> elementos;
    vector<PowerUpConf*> powerUps;
    EscenarioConf *escenario;
    AvionConf *avion;
    vector<EnemigoConf*> enemigos;

} GameConf;

#endif
