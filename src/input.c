#include <stdio.h>
#include <SDL2/SDL.h>
#include "input.h"
#include "bomberman.h"
#include "bomb.h"

#define SIZE 20

void handle_key(SDL_Event *event, BOARD *board, BOMBERMAN *bomberman, int dflag)
{
	//printf("bomberman x=%f\n y=%f\n", bomberman->x, bomberman->y);

	switch(event->key.keysym.sym)
	{
	case SDLK_DOWN:
		if(dflag){
			bomberman->direction = 0;
			bomberman->move_down = true;
			bomberman->move_up = false;
		} else {
			bomberman->move_down = false;
			bomberman->direction = bomberman->move_left ? 1 : bomberman->move_right ? 2 : bomberman->direction;
		}
		break;
	case SDLK_LEFT:
		if(dflag){
			bomberman->direction = 1;
			bomberman->move_left = true;
			bomberman->move_right = false;
		} else {
			bomberman->move_left = false;
			bomberman->direction = bomberman->move_up ? 3 : bomberman->move_down ? 0 : bomberman->direction;
		}
		break;
	case SDLK_RIGHT:
		if(dflag){
			bomberman->direction = 2;
			bomberman->move_right = true;
			bomberman->move_left = false;
		} else {
			bomberman->move_right = false;
			bomberman->direction = bomberman->move_up ? 3 : bomberman->move_down ? 0 : bomberman->direction;
		}
		break;
	case SDLK_UP:
		if(dflag){
			bomberman->direction = 3;
			bomberman->move_up = true;
			bomberman->move_down = false;
		} else {
			bomberman->move_up = false;
			bomberman->direction = bomberman->move_left ? 1 : bomberman->move_right ? 2 : bomberman->direction;
		}
		break;
	case SDLK_SPACE:
		if(dflag){
			if(can_drop_bomb(board, bomberman)){
				drop_bomb(board, bomberman);
			}
		}
		break;
	}
}

void handle_event(SDL_Event *event, int *play, BOARD *board, BOMBERMAN *bomberman)
{

	while(SDL_PollEvent(event) != 0){
		switch(event->type){
		case SDL_QUIT:
			*play = 0;
			break;
		case SDL_KEYDOWN:
			handle_key(event, board, bomberman, 1);
			break;
		case SDL_KEYUP:
			handle_key(event, board, bomberman, 0);
			break;
		}

	}
}
