/*
 * Music.h
 *
 *  Created on: 01/06/2016
 *      Author: gonzalo
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

using namespace std;

class Music {

private:
	Mix_Music* music;
	string dirDefault = "src/game/sounds/";

public:
	Music(string dir);
	virtual ~Music();
	void play();
	void play(int times);
	static void stop();
	static bool playingMusic();

};

#endif /* MUSIC_H_ */
