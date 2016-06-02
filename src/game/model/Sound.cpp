/*
 * Sound.cpp
 *
 *  Created on: 27/05/2016
 *      Author: gonzalo
 */

#include "Sound.h"

Sound::Sound(string dir) {

	string dirSound = dirDefault + dir;

	wave = Mix_LoadWAV(dirSound.c_str());
	if (wave == NULL) {
		cout << "Error al crear wave" << endl;
		return;
	}
}

void Sound::play() {
	Mix_PlayChannel( -1, wave, 0 );

}

Sound::~Sound() {
	Mix_FreeChunk(wave);
}

