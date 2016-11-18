
#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct BOMBERMAN BOMBERMAN;

struct BOARD{
    int l_size, c_size;
    int ** grid;
};
typedef struct BOARD BOARD;

BOARD* alloc_board(int l_size, int c_size);
void free_board(BOARD *board, int l_size);
int get_real_int(int val, int size);
void display_board(BOARD *board, SDL_Renderer *renderer, SDL_Texture *texture,
		SDL_Rect *wall, SDL_Rect *ground,BOMBERMAN *bomberman);

#endif // GRID_H_INCLUDED
