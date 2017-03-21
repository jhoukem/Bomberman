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
	if(bomberman->speed == 0.015f){
		bomberman->speed = 0.03f;
	}

	switch(board->grid[y][x].bonus->type){
	case MOREBOMB:
		bomberman->bomb_left++;
		break;
	case MOREPOWER:
		bomberman->bomb_power++;
		break;
	case MORESPEED:
		bomberman->speed += 0.005f;
		break;
	case MALUS:
		bomberman->speed = 0.015f;
		break;
	}

	// Remove the bonus from the map.
	free(board->grid[y][x].bonus);
	board->grid[y][x].bonus = NULL;

}
