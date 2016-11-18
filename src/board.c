
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "board.h"
#include "bomberman.h"

#define WIDTH 480
#define HEIGHT 480

#define GROUND 0
#define WALL 1
#define BOMB 2

void free_board(BOARD *board, int l_size)
{
	int i;
	for(i = 0; i < l_size; i++)
	{
		free(board->grid[i]);
	}
	free(board->grid);
	free(board);
}


BOARD* alloc_board(int l_size, int c_size)
{
	int i;
	BOARD *board = malloc(sizeof(BOARD));
	board->l_size = l_size;
	board->c_size = c_size;
	board->grid = calloc(l_size, sizeof(*board->grid)); // sizeof(*board->grid)

	for (i = 0; i < l_size; i++)
	{
		board->grid[i] = calloc(c_size, sizeof(int)); //sizeof(**grid)
	}
	board->grid[0][5] = WALL;
	return board;
}


/**
 * Display the board, change the output here.
 */
void display_board(BOARD *board, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect *wall,
		SDL_Rect *ground, BOMBERMAN *bomberman)
{
	SDL_Rect rect_to_draw;
	rect_to_draw.w = WIDTH/board->c_size;
	rect_to_draw.h = HEIGHT/board->l_size;

	//printf("w = %d h = %d\n", pos.w, pos.h);

	int i, j;
	for (i = 0; i < board->l_size; i++){
		rect_to_draw.y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			rect_to_draw.x = j * (WIDTH/board->c_size);
			if(board->grid[i][j] == GROUND){
				SDL_RenderCopy(renderer, texture, ground, &rect_to_draw);
			} else if(board->grid[i][j] == WALL){
				SDL_RenderCopy(renderer, texture, wall, &rect_to_draw);
			}
		}
	}
	rect_to_draw.x = bomberman->x * (HEIGHT/board->l_size);
	rect_to_draw.y = bomberman->y * (WIDTH/board->c_size);
	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 300) % 3;
	bomberman->sprite->x = (sprite * 15) + (bomberman->direction * 49);
	SDL_RenderCopy(renderer, texture, bomberman->sprite, &rect_to_draw);

	SDL_RenderPresent(renderer);
}

