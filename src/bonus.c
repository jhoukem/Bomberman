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
#include "assets.h"

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
		if(bomberman->bomb_power < (board->c_size/2) - 1){
			bomberman->bomb_power++;
		}
		break;
	case MORESPEED:
		if(bomberman->speed + 0.3f <= 3.0f){
			bomberman->speed += 0.3f;
		}
		break;
	case MALUS:
		bomberman->speed = 1.0f;
		break;
	case MAXPOWER:
		bomberman->bomb_power = (board->c_size/2) - 1;
		break;
	}

	// Remove the bonus from the map.
	free(board->grid[y][x].bonus);
	board->grid[y][x].bonus = NULL;
}

void spawn_bonus(BOARD *board, ASSETS *assets, int y, int x){

	int alea;
	BONUS *bonus = malloc(sizeof(*bonus));

	alea = rand()%100;
	if(alea < 10){
		bonus->type = MALUS;
		bonus->sprite = assets->malus;
	} else if(alea < 30){
		bonus->type = MORESPEED;
		bonus->sprite = assets->bonus_speed;
	} else if(alea < 60){
		bonus->type = MOREBOMB;
		bonus->sprite = assets->bonus_bomb;
	} else if(alea < 95){
		bonus->type = MOREPOWER;
		bonus->sprite = assets->bonus_power;
	} else {
		bonus->type = MAXPOWER;
		bonus->sprite = assets->bonus_max_power;
	}

	board->grid[y][x].bonus = bonus;
}
