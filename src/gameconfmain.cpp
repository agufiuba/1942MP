#include <iostream>

#include "xml/parser/GameParser.h"
#include "xml/conf/GameConf.h"
#include "string"

using namespace std;

int main() {
    GameConf *gc = GameParser::parse("gameconf.xml");

    cout << endl;
    cout << endl;

    cout << "ventana" << endl;
    cout << "   ancho " << gc->escenario->ancho << endl;
    cout << "   alto " << gc->escenario->alto << endl;
    cout << "sprites" << endl;
    for (int i = 0; i < gc->sprites.size(); i++) {
        cout << "   sprite" << endl;
        cout << "       id " << gc->sprites[i]->id << endl;
        cout << "       path " << gc->sprites[i]->path << endl;
        cout << "       ancho " << gc->sprites[i]->ancho << endl;
        cout << "       alto " << gc->sprites[i]->alto << endl;
    }
    cout << "elementos" << endl;
    for (int i = 0; i < gc->elementos.size(); i++) {
        cout << "   elemento" << endl;
        cout << "       spriteID " << gc->elementos[i]->spriteID << endl;
        cout << "       x " << gc->elementos[i]->x << endl;
        cout << "       y " << gc->elementos[i]->y << endl;
    }
    cout << "escenario" << endl;
    cout << "   ancho " << gc->escenario->ancho << endl;
    cout << "   alto " << gc->escenario->alto << endl;
    cout << "   fondo " << gc->escenario->fondo << endl;
    cout << "avion" << endl;
    cout << "   velocidadDesplazamiento " << gc->avion->velocidadDesplazamiento << endl;
    cout << "   velocidadDisparos " << gc->avion->velocidadDisparos << endl;
    cout << "   avionSpriteID " << gc->avion->avionSpriteID << endl;
    cout << "   vueltaSpriteID " << gc->avion->vueltaSpriteID << endl;
    cout << "   disparosSpriteID " << gc->avion->disparosSpriteID << endl;
    cout << "maxClients " << gc->maxClients << endl;

    int findSprite = GameParser::findSprite(gc->sprites, gc->escenario->fondo);
    cout << gc->sprites[findSprite]->path << endl;
}
