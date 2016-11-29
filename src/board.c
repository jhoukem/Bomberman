
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "board.h"
#include "bomberman.h"
#include "assets.h"
#include "bomb.h"

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
	board->grid = calloc(l_size, sizeof(*board->grid));
	srand(time(NULL));
	for (i = 0; i < l_size; i++)
	{
		board->grid[i] = calloc(c_size, sizeof(**board->grid)); //sizeof(**grid)
		board->grid[i]->bomb = NULL;
		board->grid[i]->bomberman = NULL;
		board->grid[i]->type = GROUND;
	}

	// Add random wall.
	for (i = 0; i < 25; i++){
		do{
			spawn_x = rand()%c_size;
			spawn_y = rand()%l_size;
			// Everywhere except on the center of the map.
		} while(spawn_y == l_size/2 || spawn_x == c_size/2);
		board->grid[spawn_y][spawn_x].type = WALL;
	}

	return board;
}

void update_cell(BOARD *board, int x, int y)
{
	CELL *cell;
	cell = &board->grid[y][x];

	// Si la cellule comporte une bombe
	if(cell->bomb != NULL){
		update_bomb(board, cell->bomb);
		if(cell->bomb->timer <= 0){
			if(!cell->bomb->has_explode){
				explode(board, cell->bomb);
			} else {
				free_bomb(board, cell->bomb);
			}
		} else {
			cell->bomb->timer --;
		}
	}
}

void update_board(BOARD *board, BOMBERMAN *bomberman)
{
	int i, j;
	update_bomberman(board, bomberman, board->l_size, board->c_size);

	for (i = 0; i < board->l_size; i++){
		for (j = 0; j < board->c_size; j++){
			update_cell(board, j, i);
		}
	}

}

void display_board(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, BOMBERMAN *bomberman)
{
	SDL_Rect draw_pos;
	draw_pos.w = WIDTH/board->c_size;
	draw_pos.h = HEIGHT/board->l_size;

	display_scenery(board, renderer, assets, &draw_pos);
	render_bomberman(renderer, bomberman, assets->spritesheet, &draw_pos);
	render_bombs(board, renderer, assets, &draw_pos);
	SDL_RenderPresent(renderer);
}


void display_scenery(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos)
{
	int i, j;
	for (i = 0; i < board->l_size; i++){
		draw_pos->y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			draw_pos->x = j * (WIDTH/board->c_size);
			if(board->grid[i][j].type == GROUND){
				SDL_RenderCopy(renderer, assets->spritesheet, &assets->ground, draw_pos);
			} else if(board->grid[i][j].type == WALL){
				SDL_RenderCopy(renderer, assets->spritesheet, &assets->wall, draw_pos);
			}
		}
	}
}

