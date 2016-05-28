/*
 * Sound.cpp
 *
 *  Created on: 27/05/2016
 *      Author: gonzalo
 */

#include "Sound.h"

Sound::Sound() {

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		return;

	const char* dir = "src/game/sounds/explosion.wav";

	wave = Mix_LoadWAV(dir);
	if (wave == NULL)
		return;

	// Load our music
//	music = Mix_LoadMUS(MUS_PATH);
//	if (music == NULL)
//		return;

	if (Mix_PlayChannel(-1, wave, 0) == -1)
		return;

//	if (Mix_PlayMusic(music, -1) == -1)
//		return;

}

void Sound::play() {
	while ( Mix_PlayingMusic() ) ;
}

Sound::~Sound() {
	Mix_FreeChunk(wave);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

