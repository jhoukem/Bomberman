/*
 * bonus.c
 *
 *  Created on: 19 mars 2017
 *      Author: Jean-Hugo
 */

#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "bonus.h"
#include "bomberman.h"

void apply_bonus_on_bomberman(BOARD *board, int y, int x, BOMBERMAN *bomberman)
{

	// Remove the malus if there is any.
	if(bomberman->speed == 1.0f){
		bomberman->speed = 1.5f;
	}

	switch(board->grid[y][x].bonus->type){
	case MOREBOMB:
		bomberman->bomb_left++;
		break;
	case MOREPOWER:
		bomberman->bomb_power++;
		break;
	case MORESPEED:
		if(bomberman->speed += 0.3f <= 3.0f){
			bomberman->speed += 0.3f;
		}
		break;
	case MALUS:
		bomberman->speed = 1.0f;
		break;
	}

	// Remove the bonus from the map.
	free(board->grid[y][x].bonus);
	board->grid[y][x].bonus = NULL;

}
