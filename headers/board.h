#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#endif

#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

typedef struct BOMBERMAN BOMBERMAN;
typedef struct ASSETS ASSETS;
typedef struct BOMB BOMB;
typedef struct BONUS BONUS;



struct CELL{
	int type;
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

BOARD* alloc_board(int l_size, int c_size);
void free_board(BOARD *board);
int update_board(SDL_Renderer *renderer, BOARD *board, BOMBERMAN *bomberman, ASSETS *assets);
void update_cell(BOARD *board, ASSETS *assets, int x, int y);
void display_board(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, BOMBERMAN *bomberman);
void display_scenery(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos);
void spawn_bonus(BOARD *board, ASSETS *assets, int x, int y);
#endif // GRID_H_INCLUDED
