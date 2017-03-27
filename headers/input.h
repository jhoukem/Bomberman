#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

typedef struct BOMBERMAN BOMBERMAN;
typedef struct AUDIO_PARAM AUDIO_PARAM;
typedef struct BOARD BOARD;

void handle_key(SDL_Event *event, SDL_bool *pause, SDL_bool *reset, SDL_bool game_over, BOARD *board,
		BOMBERMAN *bomberman, SDL_bool dflag);
void handle_event(SDL_Event *event, SDL_bool *play, SDL_bool *pause, SDL_bool *reset, SDL_bool game_over, BOARD *board,
		BOMBERMAN *bomberman);

#endif // INPUT_H_INCLUDED
