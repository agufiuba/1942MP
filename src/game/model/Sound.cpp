/*
 * Sound.cpp
 *
 *  Created on: 27/05/2016
 *      Author: gonzalo
 */

#include "Sound.h"

Sound::Sound() {

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
//	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		cout << "Error al realizar open audio" << endl;
		return;
	}

//	const char* dir = "src/game/sounds/explosion.wav";
	const char* dir = "src/game/sounds/AEllosAtacar.mp3";
//	const char* dir = "src/game/sounds/beat.wav";

	wave = Mix_LoadWAV(dir);
	if (wave == NULL) {
		cout << "Error al crear wave" << endl;
		return;
	}

	// Load our music
//	music = Mix_LoadMUS(dir);
//	if (music == NULL) {
//		cout << "Error al crear music" << endl;
//		return;
//	}

}

void Sound::play() {
	std::cout << "Corriendo sonido in" << endl;
//	Mix_PlayMusic( music, -1 );
	int channel = Mix_PlayChannel( -1, wave, 0 );
	std::cout << "Corriendo sonido out" << endl;

}

Sound::~Sound() {
	Mix_FreeChunk(wave);
//	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

