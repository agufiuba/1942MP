#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H
typedef struct PlayerData {
  char name[15];
  char color[10];
  int x;
  int y;
  int estacionamientoX;
  int estacionamientoY;
  int team;
  int score;
  int coopTeamScore;
  int alphaTeamScore;
  int betaTeamScore;
} PlayerData;
#endif
