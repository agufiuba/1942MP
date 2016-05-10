#ifndef INC_1942MP_GAMEPARSER_H
#define INC_1942MP_GAMEPARSER_H

#include "../../libs/tinyxml2.h"
#include "../conf/GameConf.h"
#include "../conf/SpriteConf.h"
#include "../conf/EscenarioConf.h"
#include "../conf/ElementoConf.h"
#include "../conf/AvionConf.h"

using namespace tinyxml2;
using namespace std;

class GameParser {
public:
    static GameConf *parse(string);

private:
    static vector<SpriteConf *> sprites(XMLDocument *);

    static SpriteConf *sprite(XMLElement *);

    static EscenarioConf *escenario(XMLDocument *);

//    static tuple<int, int> ventana(XMLDocument *);

    static vector<ElementoConf *> elementos(XMLElement *);

    static ElementoConf *elemento(XMLElement *);

    static AvionConf *avion(XMLDocument *);
};

#endif
