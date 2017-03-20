/*
 * bonus.c
 *
 *  Created on: 19 mars 2017
 *      Author: Jean-Hugo
 */

#include "bonus.h"
#include "bomberman.h"

void apply_bonus_on_bomberman(BONUS *bonus, BOMBERMAN *bomberman){

	switch(bonus->type){
	case MOREBOMB:
		bomberman->bomb_left++;
		break;
	case MOREPOWER:
		bomberman->bomb_power++;
		break;
	case MORESPEED:
		bomberman->speed += 0.001;
		break;
	case MALUS:
		bomberman->speed = 0.015;
		break;
	}

}
