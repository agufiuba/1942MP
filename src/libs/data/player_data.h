#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H
typedef struct PlayerData {
  char name[15];
  char color[10];
  int x;
  int y;
  int team;
  int score;
  int teamScore;
  int rivalTeamScore;
} PlayerData;
#endif
