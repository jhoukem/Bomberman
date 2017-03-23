#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#endif

#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "cell_type.h"
typedef struct BOMBERMAN BOMBERMAN;
typedef struct ASSETS ASSETS;
typedef struct BOMB BOMB;
typedef struct BONUS BONUS;
typedef struct _TTF_Font TTF_Font;

struct CELL{
	CELL_TYPE type;
	BOMB *bomb;
	BOMBERMAN *bomberman;
	BONUS *bonus;
};
typedef struct CELL CELL;

struct BOARD{
    int l_size, c_size;
    CELL ** grid;
};
typedef struct BOARD BOARD;
typedef struct GRAPHIC_PARAM GRAPHIC_PARAM;

BOARD* alloc_board(int l_size, int c_size);
void free_board(BOARD *board);
int update_board(GRAPHIC_PARAM *g_param, BOARD *board, BOMBERMAN *bomberman);
void update_cell(BOARD *board, ASSETS *assets, int y, int x);

void display_board(GRAPHIC_PARAM *g_param, BOARD *board, BOMBERMAN *bomberman);
void display_scenery(GRAPHIC_PARAM *g_param, BOARD *board);
void spawn_bonus(BOARD *board, ASSETS *assets, int y, int x);
void display_status(GRAPHIC_PARAM *g_param, int status);
#endif // GRID_H_INCLUDED
