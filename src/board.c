
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "bomberman.h"
#include "assets.h"
#include "bomb.h"
#include "ai.h"
#include "bonus.h"
#include "graphic_param.h"
#include "audio_param.h"

#define WIDTH 480
#define HEIGHT 480

#define DEBUG 0
#define NB_BOMBERMAN 4
#define NB_MAX_BONUS 20

int **grid_iteration;
DIRECTION **grid_direction;


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
	int i;

	BOARD *board = malloc(sizeof(*board));
	board->l_size = l_size;
	board->c_size = c_size;
	board->grid = malloc(l_size * sizeof(*board->grid));

	for (i = 0; i < l_size; i++){
		board->grid[i] = malloc(c_size * sizeof(**board->grid));
	}

	grid_iteration = malloc(l_size * sizeof(*grid_iteration));
	grid_direction = malloc(l_size * sizeof(*grid_direction));

	for (i = 0; i < l_size; i++){
		grid_iteration[i] = malloc(c_size * sizeof(**grid_iteration));
		grid_direction[i] = malloc(c_size * sizeof(**grid_direction));
	}

	reset_board(board);

	return board;
}

void reset_board(BOARD *board)
{
	int i, j, spawn_x, spawn_y, offset;

	srand(time(NULL));

	for (i = 0; i < board->l_size; i++){
		for (j = 0; j < board->c_size; j++){
			board->grid[i][j].bomb = NULL;
			board->grid[i][j].bonus = NULL;
			board->grid[i][j].bomberman = NULL;
			board->grid[i][j].type = GROUND;
		}
	}

	// Add borders.
	for (i = 0; i < board->l_size; i++){
		for (j = 0; j < board->c_size; j++){
			if(i == 0 || j == 0 || i == (board->l_size - 1) || j == (board->c_size - 1) ){
				board->grid[i][j].type = WALL;
			}
		}
	}
	board->grid[board->l_size/2][0].type = GROUND;
	board->grid[board->l_size/2][board->c_size - 1].type = GROUND;
	board->grid[0][board->c_size/2].type = GROUND;
	board->grid[board->l_size - 1][board->c_size/2].type = GROUND;

	offset = 3;

	// Add random wall.
	for (i = 0; i < 150; i++){
		do{
			spawn_x =   1 + rand()%(board->c_size - 2);
			spawn_y = 1 + rand()%(board->l_size - 2);
		}
		// keep the corners free.
		while((spawn_x <= 3 && spawn_y <= 3) || (spawn_x <= 3 && board->l_size - spawn_y <= offset) ||
				(spawn_y <= 3 && board->c_size - spawn_x <= offset) || (board->c_size - spawn_x <= offset &&
						board->l_size - spawn_y <= offset));
		board->grid[spawn_y][spawn_x].type = WALL_BREAKABLE;
	}

}

void spawn_bonus(BOARD *board, ASSETS *assets, int y, int x){

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

void update_cell(BOARD *board, GRAPHIC_PARAM *g_param, AUDIO_PARAM *a_param, int y, int x)
{
	CELL *cell;
	cell = &board->grid[y][x];

	// If the cell has a bomb.
	if(cell->bomb != NULL){
		update_bomb(board, cell->bomb);
		if(cell->bomb->timer <= 0){
			if(!cell->bomb->has_explode){
				explode_around(board, cell->bomb, g_param->assets, a_param->explosion);
			} else {
				free_bomb(board, cell->bomb);
			}
		}
	}
}

SDL_bool update_board(GRAPHIC_PARAM *g_param, AUDIO_PARAM *a_param, BOARD *board, BOMBERMAN *bomberman)
{
	int i, j, status;
	status = 0;

	// Player
	update_bomberman(board, bomberman, a_param);

	//Bots
	for(i = 1; i < NB_BOMBERMAN; i++){
		// Update alive bomberman.
		if(!(bomberman + i)->is_dead){
			update_ai_bomberman(board, (bomberman + i), grid_iteration, grid_direction);
			// While there is still two bombermans alive the game keep going.
			status ++;
		}
		update_bomberman(board, (bomberman + i), a_param);
	}

	status += !bomberman->is_dead;

	for (i = 0; i < board->l_size; i++){
		for (j = 0; j < board->c_size; j++){
			update_cell(board, g_param, a_param, i, j);
		}
	}

	if(status <= 1){
		return SDL_TRUE;
	} else {
		return SDL_FALSE;
	}
}

void display_board(GRAPHIC_PARAM *g_param, BOARD *board, BOMBERMAN *bomberman)
{
	int i;
	g_param->draw_pos->w = WIDTH/board->c_size;
	g_param->draw_pos->h = HEIGHT/board->l_size;

	display_scenery(g_param, board);
	for(i = 0; i < NB_BOMBERMAN; i++){
		// Render existing bomberman.
		if((bomberman + i) != NULL){
			render_bomberman(g_param->renderer, (bomberman + i), g_param->assets->spritesheet, g_param->draw_pos);
		}
	}
	render_bombs(g_param, board);
}

void display_status(GRAPHIC_PARAM *g_param, SDL_bool paused, SDL_bool game_over)
{
	if(game_over){
		g_param->draw_pos->x = WIDTH/2 - (g_param->game_over_surface->w/2);
		g_param->draw_pos->y = HEIGHT/2 - (g_param->game_over_surface->h/2);
		g_param->draw_pos->w = g_param->game_over_surface->w;
		g_param->draw_pos->h = g_param->game_over_surface->h;
		SDL_RenderCopy(g_param->renderer, g_param->game_over_texture, NULL, g_param->draw_pos);
		g_param->draw_pos->x = WIDTH/2 - (g_param->press_escape_surface->w/2);
		g_param->draw_pos->y = HEIGHT/2 + (g_param->game_over_surface->h);
		g_param->draw_pos->w = g_param->press_escape_surface->w;
		g_param->draw_pos->h = g_param->press_escape_surface->h;
		SDL_RenderCopy(g_param->renderer, g_param->press_escape_texture, NULL, g_param->draw_pos);
	} else if(paused){
		g_param->draw_pos->x = WIDTH/2 - (g_param->game_paused_surface->w/2);
		g_param->draw_pos->y = HEIGHT/2 - (g_param->game_paused_surface->h/2);
		g_param->draw_pos->w = g_param->game_paused_surface->w;
		g_param->draw_pos->h = g_param->game_paused_surface->h;
		SDL_RenderCopy(g_param->renderer, g_param->game_paused_texture, NULL, g_param->draw_pos);
	}
}

void display_scenery(GRAPHIC_PARAM *g_param, BOARD *board)
{
	int i, j;

	for (i = 0; i < board->l_size; i++){
		g_param->draw_pos->y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			g_param->draw_pos->x = j * (WIDTH/board->c_size);

			if(board->grid[i][j].bonus != NULL){
				SDL_RenderCopy(g_param->renderer, g_param->assets->spritesheet, &(board->grid[i][j].bonus->sprite),
						g_param->draw_pos);
			}
			else if(board->grid[i][j].type == GROUND){
				SDL_RenderCopy(g_param->renderer, g_param->assets->spritesheet,
						&g_param->assets->ground, g_param->draw_pos);
			} else if(board->grid[i][j].type == WALL){
				SDL_RenderCopy(g_param->renderer, g_param->assets->spritesheet,
						&g_param->assets->wall, g_param->draw_pos);
			}
			else if(board->grid[i][j].type == WALL_BREAKABLE){
				SDL_RenderCopy(g_param->renderer, g_param->assets->spritesheet
						, &g_param->assets->wall_breakable, g_param->draw_pos);
			}
		}
	}

	if(DEBUG){
		for (i = 0; i < board->l_size; i++){
			g_param->draw_pos->y = i * (HEIGHT/board->l_size);
			SDL_RenderDrawLine(g_param->renderer, 0, g_param->draw_pos->y, (board->c_size+1)*24, g_param->draw_pos->y);
		}

		for (j = 0; j < board->c_size; j++){
			g_param->draw_pos->x = j * (WIDTH/board->c_size);
			//printf("pos x = %d\n", draw_pos->x);
			SDL_RenderDrawLine(g_param->renderer, g_param->draw_pos->x, 0, g_param->draw_pos->x, ((board->l_size + 1)*24));
		}
	}
}

