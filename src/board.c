
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
#include "ai.h"
#include "bonus.h"

#define WIDTH 480
#define HEIGHT 480

#define GROUND 0
#define WALL 1
#define WALL_BREAKABLE 2
#define BOMB 3

#define DEBUG 1
#define NB_BOMBERMAN 4
#define NB_MAX_BONUS 20

int **grid_iteration;
int **grid_direction;


void free_board(BOARD *board)
{
	int i, j;

	for (i = 0; i < board->l_size; i++){
		for (j = 0; j < board->c_size; j++){
			if(board->grid[i][j].bonus != NULL){
				free(board->grid[i][j].bonus);
			}
		}
	}

	for(i = 0; i < board->l_size; i++)
	{
		free(board->grid[i]);
		free(grid_iteration[i]);
		free(grid_direction[i]);
	}
	free(grid_iteration);
	free(grid_direction);
	free(board->grid);
	free(board);
}


BOARD* alloc_board(int l_size, int c_size)
{
	int i, j, spawn_x, spawn_y, offset;
	BOARD *board = malloc(sizeof(BOARD));
	board->l_size = l_size;
	board->c_size = c_size;
	board->grid = calloc(l_size, sizeof(*board->grid));
	srand(time(NULL));
	for (i = 0; i < l_size; i++){
		board->grid[i] = calloc(c_size, sizeof(**board->grid));
		board->grid[i]->bomb = NULL;
		board->grid[i]->bomberman = NULL;
		board->grid[i]->type = GROUND;
	}


	// Add borders.
	for (i = 0; i < l_size; i++){
		for (j = 0; j < c_size; j++){
			if(i == 0 || j == 0 || i == (l_size - 1) || j == (c_size - 1) ){
				board->grid[i][j].type = WALL;
			}
		}
	}
	board->grid[l_size/2][0].type = GROUND;
	board->grid[l_size/2][c_size - 1].type = GROUND;
	board->grid[0][c_size/2].type = GROUND;
	board->grid[l_size - 1][c_size/2].type = GROUND;

	offset = 3;

	// Add random wall.
	for (i = 0; i < 50; i++){
		do{
			spawn_x =   1 + rand()%(c_size - 2);
			spawn_y = 1 + rand()%(l_size - 2);
		}
		// keep the corners free.
		while((spawn_x <= 3 && spawn_y <= 3) || (spawn_x <= 3 && l_size - spawn_y <= offset) ||
				(spawn_y <= 3 && c_size - spawn_x <= offset) || (c_size - spawn_x <= offset && l_size - spawn_y <= offset));
		board->grid[spawn_y][spawn_x].type = WALL_BREAKABLE;
	}


	grid_iteration = malloc(l_size * sizeof(*grid_iteration));
	grid_direction = malloc(l_size * sizeof(*grid_direction));

	for (i = 0; i < l_size; i++){
		grid_iteration[i] = malloc(c_size * sizeof(**grid_iteration));
		grid_direction[i] = malloc(c_size * sizeof(**grid_direction));
	}
	for (i = 0; i < l_size; i++){
		for (j = 0; j < c_size; j++){
			grid_iteration[i][j] = -1;
			grid_direction[i][j] = -1;
		}
	}

	return board;
}

void spawn_bonus(BOARD *board, ASSETS *assets, int x, int y){

	BONUS *bonus = malloc(sizeof(*bonus));

	switch(rand()%4){
	case 0:
		bonus->type = MORESPEED;
		bonus->sprite = assets->bonus_speed;
		break;
	case 1:
		bonus->type = MOREBOMB;
		bonus->sprite = assets->bonus_bomb;
		break;
	case 2:
		bonus->type = MOREPOWER;
		bonus->sprite = assets->bonus_power;
		break;
	case 3:
		bonus->type = MALUS;
		bonus->sprite = assets->malus;
		break;
	}

	board->grid[y][x].bonus = bonus;
}

void update_cell(BOARD *board, ASSETS *assets, int y, int x)
{
	CELL *cell;
	cell = &board->grid[y][x];

	// If the cell has a bomb.
	if(cell->bomb != NULL){
		update_bomb(board, cell->bomb);
		if(cell->bomb->timer <= 0){
			if(!cell->bomb->has_explode){
				explode_around(board, cell->bomb, assets);
			} else {
				free_bomb(board, cell->bomb);
			}
		} else {
			cell->bomb->timer --;
		}
	}
}

int update_board(SDL_Renderer *renderer, BOARD *board, BOMBERMAN *bomberman, ASSETS *assets)
{
	int i, j, status;
	status = -1;
	// Player
	update_bomberman(board, bomberman);

	//Bots
	for(i = 1; i < NB_BOMBERMAN; i++){
		// Update alive bomberman.
		if(!(bomberman + i)->is_dead){
			update_ai_bomberman(board, (bomberman + i), grid_iteration, grid_direction);
			// While there is still two bombermans alive the game keep going.
			status ++;
		}
	}

	status += !bomberman->is_dead;

	for (i = 0; i < board->l_size; i++){
		for (j = 0; j < board->c_size; j++){
			update_cell(board, assets, i, j);
		}
	}

	return status;
}

void display_board(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, BOMBERMAN *bomberman)
{
	int i;
	SDL_Rect draw_pos;
	draw_pos.w = WIDTH/board->c_size;
	draw_pos.h = HEIGHT/board->l_size;

	display_scenery(board, renderer, assets, &draw_pos);
	for(i = 0; i < NB_BOMBERMAN; i++){
		// Render alive bomberman.
		if(!(bomberman + i)->is_dead){
			render_bomberman(renderer, (bomberman + i), assets->spritesheet, &draw_pos);
		}
	}
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

			if(board->grid[i][j].bonus != NULL){
				SDL_RenderCopy(renderer, assets->spritesheet, &(board->grid[i][j].bonus->sprite), draw_pos);
			}
			else if(board->grid[i][j].type == GROUND){
				SDL_RenderCopy(renderer, assets->spritesheet, &assets->ground, draw_pos);
			} else if(board->grid[i][j].type == WALL){
				SDL_RenderCopy(renderer, assets->spritesheet, &assets->wall, draw_pos);
			}
			else if(board->grid[i][j].type == WALL_BREAKABLE){
				SDL_RenderCopy(renderer, assets->spritesheet, &assets->wall_breakable, draw_pos);
			}
		}
	}

	if(DEBUG){
		for (i = 0; i < board->l_size; i++){
			draw_pos->y = i * (HEIGHT/board->l_size);
			SDL_RenderDrawLine(renderer, 0, draw_pos->y, (board->c_size+1)*24, draw_pos->y);
		}

		for (j = 0; j < board->c_size; j++){
			draw_pos->x = j * (WIDTH/board->c_size);
			//printf("pos x = %d\n", draw_pos->x);
			SDL_RenderDrawLine(renderer, draw_pos->x, 0, draw_pos->x, ((board->l_size + 1)*24));
		}
	}
	/*
	void display_bonus(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos){

	}*/

}

