/*
 * Sound.cpp
 *
 *  Created on: 27/05/2016
 *      Author: gonzalo
 */

#include "Sound.h"

Sound::Sound() {

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
//	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
		cout << "Error al realizar open audio" << endl;
		return;
	}

//	const char* dir = "src/game/sounds/explosion.wav";
	const char* dir = "src/game/sounds/AEllosAtacar.wav";

	wave = Mix_LoadWAV(dir);
	if (wave == NULL) {
		cout << "Error al crear wave" << endl;
		return;
	}

	cout << "Longitud: " << wave->alen << endl;


	// Load our music
//	music = Mix_LoadMUS(dir);
//	if (music == NULL) {
//		cout << "Error al crear music" << endl;
//		return;
//	}

//	if (Mix_PlayChannel(-1, wave, 0) == -1)
//		return;

//	if (Mix_PlayMusic(music, -1) == -1)
//		return;

}

void Sound::play() {
	int channel = Mix_PlayChannel( -1, wave, 0 );
//	while ( Mix_Playing(channel) ){
//		//std::cout << "Corriendo sonido" << endl;
//	}
//	Mix_Pause(channel);
//	Mix_CloseAudio();
//	Mix_PlayMusic(music, -1);
}

Sound::~Sound() {
	Mix_FreeChunk(wave);
//	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

