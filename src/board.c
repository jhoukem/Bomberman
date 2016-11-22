
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "board.h"
#include "bomberman.h"
#include "assets.h"

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
	int i, spawn_x, spawn_y;
	BOARD *board = malloc(sizeof(BOARD));
	board->l_size = l_size;
	board->c_size = c_size;
	board->grid = calloc(l_size, sizeof(*board->grid)); // sizeof(*board->grid)
	srand(time(NULL));
	for (i = 0; i < l_size; i++)
	{
		board->grid[i] = calloc(c_size, sizeof(int)); //sizeof(**grid)
	}

	// Add random wall.
	for (i = 0; i < 25; i++){
		do{
			spawn_x = rand()%c_size;
			spawn_y = rand()%l_size;
			// Everywhere except on the center of the map.
		} while(spawn_y == l_size/2 || spawn_x == c_size/2);
		board->grid[rand()%l_size][rand()%c_size] = WALL;
	}

	return board;
}

void display_board(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, BOMBERMAN *bomberman)
{
	SDL_Rect draw_pos;
	draw_pos.w = WIDTH/board->c_size;
	draw_pos.h = HEIGHT/board->l_size;

	display_scenery(board, renderer, assets, &draw_pos);
	update_bomberman(board, bomberman, board->l_size, board->c_size);
	render_bomberman(renderer, bomberman, assets->spritesheet, &draw_pos);


	SDL_RenderPresent(renderer);
}


void display_scenery(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos)
{
	int i, j;
	for (i = 0; i < board->l_size; i++){
		draw_pos->y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			draw_pos->x = j * (WIDTH/board->c_size);
			if(board->grid[i][j] == GROUND){
				SDL_RenderCopy(renderer, assets->spritesheet, assets->ground, draw_pos);
			} else if(board->grid[i][j] == WALL){
				SDL_RenderCopy(renderer, assets->spritesheet, assets->wall, draw_pos);
			}
		}
	}
}

