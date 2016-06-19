#ifndef ENEMY_STATUS_H
#define ENEMY_STATUS_H
typedef struct EnemyStatus {
  int id;
  int x;
  int y;
  int offset;
  char type;
  char status;
  char playerID[15];
} EnemyStatus;
#endif
