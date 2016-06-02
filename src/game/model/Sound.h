/*
 * Sound.h
 *
 *  Created on: 27/05/2016
 *      Author: gonzalo
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

using namespace std;

class Sound {
private:
	string dirDefault = "src/game/sounds/";
	Mix_Chunk *wave = NULL;

public:
	Sound(string dir);
	void play();
	virtual ~Sound();
};

#endif /* SOUND_H_ */
