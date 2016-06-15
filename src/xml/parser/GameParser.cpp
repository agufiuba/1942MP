#include "GameParser.h"

#include <iostream>
#include "tuple"

GameConf *GameParser::parse(string fn) {
    XMLDocument doc;
    GameConf *gc = new GameConf;
    doc.LoadFile(fn.c_str());
    gc->sprites = sprites(&doc);
    gc->escenario = escenario(&doc, gc);
    gc->avion = avion(&doc);
    gc->enemigos = enemigos(&doc);
    int mc, jugadoresPorEquipo;
    doc.FirstChildElement("maxClients")->QueryIntText(&mc);
    doc.FirstChildElement("jugadoresPorEquipo")->QueryIntText(&jugadoresPorEquipo);
    gc->maxClients = mc;
    gc->jugadoresPorEquipo = jugadoresPorEquipo;

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
    XMLElement *anchoElement = e->FirstChildElement("ancho");
    XMLElement *altoElement = e->FirstChildElement("alto");
    string id = idElement->GetText();
    strcpy(spriteConf->id, id.c_str());
    string path = pathElement->GetText();
    path = path.substr(1, path.size() - 2);
    strcpy(spriteConf->path, path.c_str());
    int alto, ancho;
    altoElement->QueryIntText(&alto);
    anchoElement->QueryIntText(&ancho);
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

EscenarioConf *GameParser::escenario(XMLDocument *doc, GameConf *gc) {
    EscenarioConf *ec = new EscenarioConf;
    XMLElement *escenarioE = doc->FirstChildElement("escenario");
    XMLElement *altoE = escenarioE->FirstChildElement("alto");
    XMLElement *anchoE = escenarioE->FirstChildElement("ancho");
    XMLElement* nivelE = escenarioE->FirstChildElement("nivel");
    XMLElement* longitudNivelE = nivelE->FirstChildElement("longitudNivel");
    XMLElement* cantidadNivelesE = nivelE->FirstChildElement("cantidadNiveles");

    int ancho, alto, longitudNivel, cantidadNiveles;
    altoE->QueryIntText(&alto);
    anchoE->QueryIntText(&ancho);
    longitudNivelE->QueryIntText(&longitudNivel);
    cantidadNivelesE->QueryIntText(&cantidadNiveles);

    string fondoSpriteID = escenarioE->FirstChildElement("fondo")->FirstChildElement("spriteID")->GetText();
    strcpy(ec->fondo, fondoSpriteID.c_str());

    ec->alto = alto;
    ec->ancho = ancho;
    ec->longitudNivel = longitudNivel;
    ec->cantidadNiveles = cantidadNiveles;
    gc->elementos = elementos(escenarioE);
    gc->powerUps = powerUps(escenarioE);

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
    strcpy(elementoConf->spriteID, spriteID.c_str());
    elementoConf->x = x;
    elementoConf->y = y;
    return elementoConf;
}

vector<PowerUpConf*> GameParser::powerUps(XMLElement *e) {
    vector<PowerUpConf*> powerUps;
    XMLElement *powerUpsE = e->FirstChildElement("powerUps");
    for (XMLElement *powerUpE = powerUpsE->FirstChildElement("powerUp");
         powerUpE != NULL;
         powerUpE = powerUpE->NextSiblingElement("powerUp")) {
    	powerUps.push_back(powerUp(powerUpE));
    }
    return powerUps;
}

PowerUpConf* GameParser::powerUp(XMLElement *e) {
    PowerUpConf* powerUpConf = new PowerUpConf;
    string tipo = e->FirstChildElement("tipo")->GetText();
    int x, y;
    e->FirstChildElement("posicion")->FirstChildElement("x")->QueryIntText(&x);
    e->FirstChildElement("posicion")->FirstChildElement("y")->QueryIntText(&y);
    strcpy(powerUpConf->tipo, tipo.c_str());
    powerUpConf->x = x;
    powerUpConf->y = y;
    return powerUpConf;
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
    strcpy(avionConf->avionSpriteID, avionSpriteID.c_str());
    strcpy(avionConf->vueltaSpriteID, vueltaSpriteID.c_str());
    strcpy(avionConf->disparosSpriteID, disparosSpriteID.c_str());
    avionConf->velocidadDesplazamiento = velocidadDesplazamiento;
    avionConf->velocidadDisparos = velocidadDisparos;
    return avionConf;
}

vector<EnemigoConf*> GameParser::enemigos(XMLDocument *e) {
	vector<EnemigoConf*> enemigos;
	XMLElement *enemigosE = e->FirstChildElement("enemigos");
	for (XMLElement* enemigoE = enemigosE->FirstChildElement("enemigo");
		 enemigoE != NULL;
		 enemigoE = enemigoE->NextSiblingElement("enemigo")) {
			enemigos.push_back(enemigo(enemigoE));
	}
	return enemigos;
}

EnemigoConf* GameParser::enemigo(XMLElement* e) {
	EnemigoConf* enemigoConf = new EnemigoConf;
	string tipo = e->FirstChildElement("tipo")->GetText();
	int x, y, velocidadDisparos;
	e->FirstChildElement("posicion")->FirstChildElement("x")->QueryIntText(&x);
	e->FirstChildElement("posicion")->FirstChildElement("y")->QueryIntText(&y);
	e->FirstChildElement("velocidadDisparos")->QueryIntText(&velocidadDisparos);
	strcpy(enemigoConf->tipo, tipo.c_str());
	enemigoConf->x = x;
	enemigoConf->y = y;
	enemigoConf->velocidadDisparos = velocidadDisparos;
	return enemigoConf;
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
