/*
 * game_data.h
 *
 *  Created on: 2 de jun. de 2016
 *      Author: ramon
 */

#ifndef SRC_LIBS_DATA_GAME_DATA_H_
#define SRC_LIBS_DATA_GAME_DATA_H_
typedef struct GameData {
  bool teamMode;
  bool cooperativeMode;
  bool practiceMode;
  int maxPlayersTeams;
  int countPlayersTeam1;
  int countPlayersTeam2;
} GameData;
#endif /* SRC_LIBS_DATA_GAME_DATA_H_ */
