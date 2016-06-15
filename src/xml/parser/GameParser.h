#ifndef INC_1942MP_GAMEPARSER_H
#define INC_1942MP_GAMEPARSER_H

#include "../../libs/tinyxml2.h"
#include "../conf/GameConf.h"
#include "../conf/SpriteConf.h"
#include "../conf/EscenarioConf.h"
#include "../conf/ElementoConf.h"
#include "../conf/PowerUpConf.h"
#include "../conf/AvionConf.h"
#include "../conf/EnemigoConf.h"

using namespace tinyxml2;
using namespace std;

class GameParser {
public:
    static GameConf *parse(string);

    static int findSprite(vector<SpriteConf *>, string);

private:
    static vector<SpriteConf *> sprites(XMLDocument *);

    static SpriteConf *sprite(XMLElement *);

    static EscenarioConf *escenario(XMLDocument *, GameConf *gc);

    static vector<ElementoConf *> elementos(XMLElement *);

    static ElementoConf *elemento(XMLElement *);

    static vector<PowerUpConf*> powerUps(XMLElement *);

    static PowerUpConf* powerUp(XMLElement *);

    static AvionConf *avion(XMLDocument *);

    static vector<EnemigoConf*> enemigos(XMLDocument *);

    static EnemigoConf* enemigo(XMLElement *);
};

#endif
