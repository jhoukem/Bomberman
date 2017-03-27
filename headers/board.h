#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include "cell_type.h"
typedef struct BOMBERMAN BOMBERMAN;
typedef struct ASSETS ASSETS;
typedef struct BOMB BOMB;
typedef struct BONUS BONUS;
typedef struct _TTF_Font TTF_Font;
typedef struct AUDIO_PARAM AUDIO_PARAM;

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
void reset_board(BOARD *board);
void free_board(BOARD *board);
SDL_bool update_board(GRAPHIC_PARAM *g_param, AUDIO_PARAM *a_param, BOARD *board, BOMBERMAN *bomberman);
void update_cell(BOARD *board, GRAPHIC_PARAM *g_param, AUDIO_PARAM *a_param, int y, int x);
void spawn_bonus(BOARD *board, ASSETS *assets, int y, int x);
void display_board(GRAPHIC_PARAM *g_param, BOARD *board, BOMBERMAN *bomberman);
void display_scenery(GRAPHIC_PARAM *g_param, BOARD *board);
void display_status(GRAPHIC_PARAM *g_param, SDL_bool paused, SDL_bool game_over);
#endif // GRID_H_INCLUDED
