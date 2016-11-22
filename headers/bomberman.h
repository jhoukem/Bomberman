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
    int direction;
    Bool is_moving;
    SDL_Rect *sprite;
};
typedef struct BOMBERMAN BOMBERMAN;

BOMBERMAN* alloc_bomberman(BOARD *board);
void update_bomberman(BOMBERMAN *bomberman, SDL_Rect *draw_pos, int l_size, int c_size);
void update_position(BOMBERMAN *bomberman);
void update_animation(BOMBERMAN *bomberman);
void free_bomberman(BOMBERMAN * bomberman);
float get_next_int(float val, int size);
#endif /* BOMBERMAN_H_ */
