#ifndef INC_1942MP_ESCENARIOCONF_H
#define INC_1942MP_ESCENARIOCONF_H

#include "ElementoConf.h"

class EscenarioConf {
public:
    int ancho;
    int alto;
    string fondo;
    vector<ElementoConf *> elementos;
};

#endif
