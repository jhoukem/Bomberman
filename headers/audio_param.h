/*
 * audio_param.h
 *
 *  Created on: 26 mars 2017
 *      Author: Jean-Hugo
 */

#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef AUDIO_PARAM_H_
#define AUDIO_PARAM_H_
struct AUDIO_PARAM{
	int nb_ambiance;
	Mix_Music **ambiance;
	Mix_Chunk *explosion;
	Mix_Chunk *power_up;
};
typedef struct AUDIO_PARAM AUDIO_PARAM;

#endif /* AUDIO_PARAM_H_ */
