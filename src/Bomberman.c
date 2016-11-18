/*
 * bomberman.c
 *
 *  Created on: 17 nov. 2016
 *      Author: Jean-Hugo
 */

#include "board.h"
#include "bomberman.h"
#include <stdlib.h>


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

	return bomberman;
}

void free_bomberman(BOMBERMAN *bomberman)
{
	free(bomberman->sprite);
	free(bomberman);
}
