
#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "Bool.h"
typedef struct BOMBERMAN BOMBERMAN;
typedef struct ASSETS ASSETS;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct BOMB BOMB;



struct CELL{
	int type;
	Bool is_under_explosion;
	BOMB *bomb;
	BOMBERMAN *bomberman;
};

typedef struct CELL CELL;

struct BOARD{
    int l_size, c_size;
    CELL ** grid;
};
typedef struct BOARD BOARD;

BOARD* alloc_board(int l_size, int c_size);
void free_board(BOARD *board, int l_size);
int get_real_int(int val, int size);
void update_board(BOARD *board, BOMBERMAN *bomberman);
void update_cell(BOARD *board, int x, int y);
void display_board(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, BOMBERMAN *bomberman);
void display_scenery(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos);
#endif // GRID_H_INCLUDED
