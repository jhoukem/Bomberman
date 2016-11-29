/*
 * bomberman.h
 *
 *  Created on: 17 nov. 2016
 *      Author: Jean-Hugo
 */

#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

#include "bool.h"

typedef struct BOARD BOARD;
typedef struct SDL_Rect SDL_Rect;

struct BOMBERMAN{
    float x, y, speed;
    int direction, bomb_left, bomb_power;
    Bool move_down, move_left, move_right, move_up;
    SDL_Rect sprite;
};
typedef struct BOMBERMAN BOMBERMAN;

BOMBERMAN* alloc_bomberman(BOARD *board);
void update_bomberman(BOARD *board, BOMBERMAN *bomberman, int l_size, int c_size);
void update_position(BOARD *board, BOMBERMAN *bomberman);
void update_bomberman_animation(BOMBERMAN *bomberman);
void render_bomberman(SDL_Renderer *renderer, BOMBERMAN *bomberman, SDL_Texture *spritesheet, SDL_Rect *draw_pos);
void free_bomberman(BOMBERMAN * bomberman);
int can_go_over(BOARD *board, BOMBERMAN *bomberman, int next_y_in_tab, int next_x_in_tab);
Bool is_moving(BOMBERMAN *bomberman);
float get_next_int(float val, int size);
int from_pixel_to_grid(BOARD *board, float pos, int fwidth);
#endif /* BOMBERMAN_H_ */
