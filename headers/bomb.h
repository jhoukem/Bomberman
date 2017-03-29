/*
 * bomb.h
 *
 *  Created on: 24 nov. 2016
 *      Author: Jean-Hugo
 */
#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef BOMB_H_
#define BOMB_H_
#include "direction.h"
typedef struct BOMBERMAN BOMBERMAN;
typedef struct BOARD BOARD;
typedef struct ASSETS ASSETS;
typedef struct Mix_Chunk Mix_Chunk;

/**
 * up, down, left, right represent where the bomb rendering need to stop for each direction.
 */
struct BOMB{
	int x, y, power, timer, up, down, left, right;
	int *bomberman_bomb_left;
	SDL_bool has_explode;
	SDL_Rect sprite;
};

typedef struct BOMB BOMB;
typedef struct GRAPHIC_PARAM GRAPHIC_PARAM;

BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left);
int can_drop_bomb(BOARD *board, BOMBERMAN *bomberman);
void drop_bomb(BOARD *board, BOMBERMAN *bomberman);
void update_bomb(BOARD *board, BOMB *bomb);
void update_bomb_animation(BOMB *bomb);
void explode_around(BOARD *board, BOMB *bomb, ASSETS *assets, Mix_Chunk *sound_explosion);
void update_damages(BOARD *board, BOMB *bomb);
void render_bombs(GRAPHIC_PARAM *g_param, BOARD *board);
void free_bomb(BOARD *board, BOMB *bomb);

void function_around_bomb(BOARD *board, BOMB *bomb, void *parameters,
		SDL_bool (*function)(BOARD *board, BOMB *bomb, int current_y, int current_x, int counter_pos,
				DIRECTION direction, void *extra_parameters));
SDL_bool explode_cell(BOARD *board, BOMB *bomb, int current_y, int current_x, int counter_pos,
		DIRECTION direction, void *extra_parameters);
SDL_bool handle_damages(BOARD *board, BOMB *bomb, int current_y, int current_x, int counter_pos,
		DIRECTION direction, void *extra_parameters);
SDL_bool render_bomb(BOARD *board, BOMB *bomb, int current_y, int current_x, int counter_pos,
		DIRECTION direction, void *extra_parameters);
#endif /* BOMB_H_ */
