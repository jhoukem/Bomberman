/*
 * bomberman.h
 *
 *  Created on: 17 nov. 2016
 *      Author: Jean-Hugo
 */

#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

typedef struct BOARD BOARD;

struct BOMBERMAN{
    int x, y;
    int direction;
    SDL_Rect *sprite;
};
typedef struct BOMBERMAN BOMBERMAN;

BOMBERMAN* alloc_bomberman(BOARD *board);
void free_bomberman(BOMBERMAN * bomberman);
#endif /* BOMBERMAN_H_ */
