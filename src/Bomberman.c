/*
 * bomberman.c
 *
 *  Created on: 17 nov. 2016
 *      Author: Jean-Hugo
 */
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "board.h"
#include "bomberman.h"

#define WIDTH 480
#define HEIGHT 480

#define FRAME_PER_ANIMATION 3
#define SPRITE_WIDTH 15
#define ANIMATION_WIDTH 49
#define ANIMATION_SPEED 250
#define SPEED 0.02

void update_bomberman(BOMBERMAN *bomberman, SDL_Rect *draw_pos, int l_size, int c_size)
{
	update_position(bomberman);
	draw_pos->x = bomberman->x ;
	draw_pos->y = bomberman->y ;
	update_animation(bomberman);
}

void update_position(BOMBERMAN *bomberman)
{
	if(bomberman->is_moving){
		switch(bomberman->direction){
		case 0:
			bomberman->y = get_next_int(bomberman->y + bomberman->speed, HEIGHT);
			break;
		case 1:
			bomberman->x = get_next_int(bomberman->x - bomberman->speed, WIDTH);
			break;
		case 2:
			bomberman->x = get_next_int(bomberman->x + bomberman->speed, WIDTH);
			break;
		case 3:
			bomberman->y = get_next_int(bomberman->y - bomberman->speed, HEIGHT);
			break;
		}
	}
}


float get_next_int(float val, int size)
{
	if(val < 0){
		return size;
	}
	else if(val > size) {
		return 0;
	}
	return val;
}


void update_animation(BOMBERMAN *bomberman)
{
	Uint32 sprite = (SDL_GetTicks() / ANIMATION_SPEED) % FRAME_PER_ANIMATION;
	if(bomberman->is_moving){
		bomberman->sprite->x = (sprite * SPRITE_WIDTH) + (bomberman->direction * ANIMATION_WIDTH);
	} else {
		bomberman->sprite->x = bomberman->direction * ANIMATION_WIDTH;
	}
}

BOMBERMAN* alloc_bomberman(BOARD *board)
{
	BOMBERMAN *bomberman;
	bomberman = malloc(sizeof(BOMBERMAN));
	bomberman->sprite = malloc(sizeof(SDL_Rect));
	bomberman->sprite->w = 15;
	bomberman->sprite->h = 24;
	bomberman->sprite->x = 0;
	bomberman->sprite->y = 0;
	bomberman->x = board->c_size/2;
	bomberman->y = board->l_size/2;
	bomberman->direction = 0;
	bomberman->is_moving = false;
	bomberman->speed = SPEED;

	return bomberman;
}

void free_bomberman(BOMBERMAN *bomberman)
{
	free(bomberman->sprite);
	free(bomberman);
	bomberman->sprite = NULL;
	bomberman = NULL;
}
