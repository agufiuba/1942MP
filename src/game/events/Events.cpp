#ifndef SRC_EVENTO_H
#define SRC_EVENTO_H

#define DERECHA 'R'
#define IZQUIERDA 'L'
#define ARRIBA 'U'
#define ABAJO 'D'
#define DISPARO 'S'
#define VUELTERETA 'E'
#define LISTO 'M'
#define REINICIAR 'N'
#define SALIR 'Q'
#define MENSAJE_CORRECTO 'C'
#define MENSAJE_INCORRECTO 'I'

#include <iostream>

using namespace std;

typedef struct Evento {
    char value;
    char name[15];
} Evento;

#endif
