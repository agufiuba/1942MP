#ifndef SRC_EVENTO_H
#define SRC_EVENTO_H

#define DERECHA 'R'
#define IZQUIERDA 'L'
#define ARRIBA 'U'
#define ABAJO 'D'
#define DISPARO 'S'
#define VUELTERETA 'E'
#define LISTO 'M'
#define REINICIAR 'T'
#define SALIR 'Q'
#define MENSAJE_CORRECTO 'C'
#define MENSAJE_INCORRECTO 'I'
#define AMETRALLADORA 'A'
#define DESTROY 'X'
#define ESTACIONO 'P'
#define EXPLOSION 'W'
#define POWERUPDESTROY 'Y'
#define QUITGAME 'Z'
#define OUTPRACTICE 'O'

#include <iostream>

using namespace std;

typedef struct Evento {
    char value;
    char name[15];
} Evento;

#endif
