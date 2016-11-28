/*
 * bomb.h
 *
 *  Created on: 24 nov. 2016
 *      Author: Jean-Hugo
 */

#ifndef BOMB_H_
#define BOMB_H_
#include "bool.h"
typedef struct SDL_Rect SDL_Rect;
typedef struct BOMBERMAN BOMBERMAN;
typedef struct BOARD BOARD;


struct BOMB{
	int x, y, power, timer;
	int *bomberman_bomb_left;
	Bool has_explode;
	SDL_Rect sprite;
};

typedef struct BOMB BOMB;


BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left);
int can_drop_bomb(BOARD *board, BOMBERMAN *bomberman);
void drop_bomb(BOARD *board, BOMBERMAN *bomberman);
void explode(BOARD *board, BOMB *bomb);
void free_bomb(BOARD *board, BOMB *bomb);

#endif /* BOMB_H_ */
