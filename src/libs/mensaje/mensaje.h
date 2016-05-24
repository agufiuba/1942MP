#ifndef SRC_MENSAJE_MENSAJE_H
#define SRC_MENSAJE_MENSAJE_H

typedef struct Mensaje {
  char id[10];
  char tipo[7];
  char valor[100];
} Mensaje;

#endif
