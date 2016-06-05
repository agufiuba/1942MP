/*
 * Music.h
 *
 *  Created on: 01/06/2016
 *      Author: gonzalo
 */

#include "Music.h"

Music::Music(string dir) {

	string dirMusic = dirDefault + dir;

	music = Mix_LoadMUS(dirMusic.c_str());
	if (music == NULL) {
		cout << "Error al crear music" << endl;
		return;
	}

}

void Music::stop() {
	Mix_HaltMusic();
}

void Music::fadeOut(int ms) {
	Mix_FadeOutMusic(ms);
}

void Music::play() {
	Mix_PlayMusic(music, -1);
}

void Music::play(int times) {
	Mix_PlayMusic(music, times);
}

bool Music::playingMusic() {
	return (Mix_PlayingMusic() != 0);
}

Music::~Music() {
	Mix_FreeMusic(music);
}

