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
//	Uint32 wav_length; // length of our sample
//	Uint8 *wav_buffer; // buffer containing our audio file
//	SDL_AudioSpec wav_spec; // the specs of our piece of music
//
//	Uint8 *audio_pos; // global pointer to the audio buffer to be played
//	Uint32 audio_len;
//
//	void my_audio_callback(void *userdata, Uint8 *stream, int len);

	Mix_Chunk *wave = NULL;
	// Our music file
	Mix_Music *music = NULL;

public:
	Sound();
	void play();
	virtual ~Sound();
};

#endif /* SOUND_H_ */
