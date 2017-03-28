/*
 * bomberman.h
 *
 *  Created on: 17 nov. 2016
 *      Author: Jean-Hugo
 */
#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_
#include "direction.h"
typedef struct BOARD BOARD;
typedef struct AUDIO_PARAM AUDIO_PARAM;

struct BOMBERMAN{
	int id;
    float x, y, speed;
    int bomb_left, bomb_power;
    int x_goal, y_goal;
    DIRECTION direction;
    SDL_bool move_down, move_left, move_right, move_up, is_dead;
    SDL_Rect sprite;
    SDL_Rect hitbox;
};
typedef struct BOMBERMAN BOMBERMAN;

BOMBERMAN* alloc_bomberman(BOARD *board);
void reset_bomberman(BOMBERMAN *bomberman, BOARD *board);
void update_bomberman(BOARD *board, BOMBERMAN *bomberman, AUDIO_PARAM *a_param);
void update_position(BOARD *board, BOMBERMAN *bomberman);
void update_bomberman_animation(BOMBERMAN *bomberman);
void render_bomberman(SDL_Renderer *renderer, BOMBERMAN *bomberman, SDL_Texture *spritesheet, SDL_Rect *draw_pos);
void free_bomberman(BOMBERMAN * bomberman);
int can_go_over(BOARD *board, BOMBERMAN *bomberman, float next_y, float next_x,
		int next_y_in_tab, int next_x_in_tab);
SDL_bool is_moving(BOMBERMAN *bomberman);
float get_next_val(float val, float size);
int from_pixel_to_grid_coord(BOARD *board, float pos, int fwidth);
int hitbox_collide(BOARD *board, BOMBERMAN *bomberman, float next_y, float next_x,
		int next_y_in_tab, int next_x_in_tab);

#endif /* BOMBERMAN_H_ */
