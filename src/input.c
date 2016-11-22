#include <stdio.h>
#include <SDL2/SDL.h>
#include "input.h"
#include "bomberman.h"

#define SIZE 20
#define WH 25


void handle_key(SDL_Event *event, BOMBERMAN *bomberman, int dflag)
{
	//printf("bomberman x=%f\n y=%f\n", bomberman->x, bomberman->y);
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// At least one moving key is pressed.
	if(state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_UP] ||
			state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT]){
		bomberman->is_moving = true;
	} else {
		bomberman->is_moving = false;
	}

	switch(event->key.keysym.sym)
	{
	case SDLK_DOWN:
		bomberman->direction = dflag ? 0 : bomberman->direction;
		break;
	case SDLK_LEFT:
		bomberman->direction = dflag ? 1 : bomberman->direction;
		break;
	case SDLK_RIGHT:
		bomberman->direction = dflag ? 2 : bomberman->direction;
		break;
	case SDLK_UP:
		bomberman->direction = dflag ? 3 : bomberman->direction;
		break;
	}
}

void handle_event(SDL_Event *event, int *play, BOMBERMAN *bomberman)
{

	while(SDL_PollEvent(event) != 0){
		switch(event->type){
		case SDL_QUIT:
			*play = 0;
			break;
		case SDL_KEYDOWN:
			handle_key(event, bomberman, 1);
			break;
		case SDL_KEYUP:
			handle_key(event, bomberman, 0);
			break;
		}

	}
}
