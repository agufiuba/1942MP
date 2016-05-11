#include "GameParser.h"

#include "tuple"

GameConf *GameParser::parse(string fn) {
    XMLDocument doc;
    GameConf *gc = new GameConf;
    doc.LoadFile(fn.c_str());
    gc->sprites = sprites(&doc);
//    tuple<int, int> anchoalto = ventana(&doc);
//    gc->ventanaAncho = get<0>(anchoalto);
//    gc->ventanaAlto = get<1>(anchoalto);
    gc->escenario = escenario(&doc);
    gc->avion = avion(&doc);
    return gc;
}

vector<SpriteConf *> GameParser::sprites(XMLDocument *doc) {
    vector<SpriteConf *> sprites;
    XMLElement *spritesE = doc->FirstChildElement("sprites");
    for (XMLElement *spriteElement = spritesE->FirstChildElement("sprite");
         spriteElement != NULL;
         spriteElement = spriteElement->NextSiblingElement("sprite")) {
        sprites.push_back(sprite(spriteElement));
    }
    return sprites;
}

SpriteConf *GameParser::sprite(XMLElement *e) {
    SpriteConf *spriteConf = new SpriteConf;
    XMLElement *idElement = e->FirstChildElement("id");
    XMLElement *pathElement = e->FirstChildElement("path");
    XMLElement *cantidadElement = e->FirstChildElement("cantidad");
    XMLElement *anchoElement = e->FirstChildElement("ancho");
    XMLElement *altoElement = e->FirstChildElement("alto");
    spriteConf->id = idElement->GetText();
    spriteConf->path = pathElement->GetText();
    spriteConf->path = spriteConf->path.substr(1, spriteConf->path.size() - 2);
    int cantidad, alto, ancho;
    cantidadElement->QueryIntText(&cantidad);
    altoElement->QueryIntText(&alto);
    anchoElement->QueryIntText(&ancho);
    spriteConf->cantidad = cantidad;
    spriteConf->alto = alto;
    spriteConf->ancho = ancho;
    return spriteConf;
}

//tuple<int, int> GameParser::ventana(XMLDocument *doc) {
//    XMLElement *ventanaE = doc->FirstChildElement("ventana");
//    XMLElement *altoE = ventanaE->FirstChildElement("alto");
//    XMLElement *anchoE = ventanaE->FirstChildElement("ancho");
//    int ancho, alto;
//    altoE->QueryIntText(&alto);
//    anchoE->QueryIntText(&ancho);
//    return make_tuple(ancho, alto);
//}

EscenarioConf *GameParser::escenario(XMLDocument *doc) {
    EscenarioConf *ec = new EscenarioConf;
    XMLElement *escenarioE = doc->FirstChildElement("escenario");
    XMLElement *altoE = escenarioE->FirstChildElement("alto");
    XMLElement *anchoE = escenarioE->FirstChildElement("ancho");
    int ancho, alto;
    altoE->QueryIntText(&alto);
    anchoE->QueryIntText(&ancho);
    string fondoSpriteID = escenarioE->FirstChildElement("fondo")->FirstChildElement("spriteID")->GetText();
    ec->fondo = fondoSpriteID;
    ec->alto = alto;
    ec->ancho = ancho;
    ec->elementos = elementos(escenarioE);
    return ec;
}

vector<ElementoConf *> GameParser::elementos(XMLElement *e) {
    vector<ElementoConf *> elementos;
    XMLElement *elementosE = e->FirstChildElement("elementos");
    for (XMLElement *elementoE = elementosE->FirstChildElement("elemento");
         elementoE != NULL;
         elementoE = elementoE->NextSiblingElement("elemento")) {
        elementos.push_back(elemento(elementoE));
    }
    return elementos;
}

ElementoConf *GameParser::elemento(XMLElement *e) {
    ElementoConf *elementoConf = new ElementoConf;
    string spriteID = e->FirstChildElement("spriteID")->GetText();
    int x, y;
    e->FirstChildElement("posicion")->FirstChildElement("x")->QueryIntText(&x);
    e->FirstChildElement("posicion")->FirstChildElement("y")->QueryIntText(&y);
    elementoConf->spriteID = spriteID;
    elementoConf->x = x;
    elementoConf->y = y;
    return elementoConf;
}

AvionConf *GameParser::avion(XMLDocument *doc) {
    AvionConf *avionConf = new AvionConf;
    XMLElement *avionE = doc->FirstChildElement("avion");
    string avionSpriteID = avionE->FirstChildElement("avionSpriteID")->GetText();
    string vueltaSpriteID = avionE->FirstChildElement("vueltaSpriteID")->GetText();
    string disparosSpriteID = avionE->FirstChildElement("disparosSpriteID")->GetText();
    int velocidadDesplazamiento, velocidadDisparos;
    avionE->FirstChildElement("velocidadDesplazamiento")->QueryIntText(&velocidadDesplazamiento);
    avionE->FirstChildElement("velocidadDisparos")->QueryIntText(&velocidadDisparos);
    avionConf->avionSpriteID = avionSpriteID;
    avionConf->vueltaSpriteID = vueltaSpriteID;
    avionConf->disparosSpriteID = disparosSpriteID;
    avionConf->velocidadDesplazamiento = velocidadDesplazamiento;
    avionConf->velocidadDisparos = velocidadDisparos;
    return avionConf;
}

int GameParser::findSprite(vector<SpriteConf *> sprites, string spriteID) {
    int i = 0;
    SpriteConf *s;
    bool found = false;
    while (i < sprites.size() && !found) {
        s = sprites[i];
        if (s->id == spriteID)
            found = true;
        i++;
    }
    if (found)
        return i - 1;
    else
        return -1;
}
