#ifndef SRC_EVENTO_H
#define SRC_EVENTO_H

#define DERECHA 'R'
#define IZQUIERDA 'L'
#define ARRIBA 'U'
#define ABAJO 'D'
#define DISPARO 'S'
#define LISTO 'M'
#define REINICIAR 'N'
#define SALIR 'Q'
#define MENSAJE_CORRECTO 'Y'
#define MENSAJE_INCORRECTO 'V'

#include <iostream>

using namespace std;

typedef struct Evento {
    char playerID;
    char value;
} Evento;

#endif
