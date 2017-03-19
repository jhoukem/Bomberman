/*
 * bomb.h
 *
 *  Created on: 24 nov. 2016
 *      Author: Jean-Hugo
 */
#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#endif

#ifndef BOMB_H_
#define BOMB_H_
typedef struct BOMBERMAN BOMBERMAN;
typedef struct BOARD BOARD;
typedef struct ASSETS ASSETS;

struct BOMB{
	int x, y, power, timer;
	int *bomberman_bomb_left;
	SDL_bool has_explode;
	SDL_Rect sprite;
};

typedef struct BOMB BOMB;


BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left);
int can_drop_bomb(BOARD *board, BOMBERMAN *bomberman);
void drop_bomb(BOARD *board, BOMBERMAN *bomberman);

void explode_around(BOARD *board, BOMB *bomb, ASSETS *assets);
SDL_bool explode_cell(BOARD *board, ASSETS *assets, int y, int x);

void free_bomb(BOARD *board, BOMB *bomb);
void render_bombs(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos);
void update_bomb(BOARD *board, BOMB *bomb);
void update_bomb_animation(BOMB *bomb);

void function_around_bomb(BOARD *board, BOMB *bomb, ASSETS *assets,
		SDL_bool (*function)(BOARD *board, ASSETS *assets, int y, int x));

#endif /* BOMB_H_ */
