/*
 * ai.c
 *
 *  Created on: 19 déc. 2016
 *      Author: Jean-Hugo
 */

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "bomberman.h"
#include "ai.h"
#include "bomb.h"

#define WIDTH 480
#define HEIGHT 480

#define GROUND 0
#define WALL 1
#define WALL_BREAKABLE 2
#define BOMB 3


void update_ai_bomberman(BOARD *board, BOMBERMAN *bomberman, int l_size, int c_size)
{
	int x, y;

	x = from_pixel_to_grid_coord(board, bomberman->x, 1);
	y = from_pixel_to_grid_coord(board, bomberman->y, 0);



	// Offensive mode -- DROP BOMB
	if(!is_dangerous_area(board, y, x)){
		if(rand()%10000 == 0 && can_drop_bomb(board, bomberman)){
			drop_bomb(board, bomberman);
		}
	}

	printf("x_goal=%d, y_goal=%d\n", bomberman->x_goal, bomberman->y_goal);
	printf("x=%d, y=%d\n", (int)bomberman->x, (int)bomberman->y);

	// On initialization.
	if((bomberman->y_goal == 0 && bomberman->x_goal == 0) ||
			// Reached his goal.
			(bomberman->y_goal == (int) bomberman->y && bomberman->x_goal == (int) bomberman->x)){
		set_new_goal(board, bomberman, y, x);
	}

	else if(bomberman->x_goal > (int)bomberman->x){
		bomberman->direction = 2;
		bomberman->move_right = SDL_TRUE;
		bomberman->move_left = SDL_FALSE;
	}
	else if(bomberman->x_goal < (int)bomberman->x){
		bomberman->direction = 1;
		bomberman->move_left = SDL_TRUE;
		bomberman->move_right = SDL_FALSE;
	}
	else if(bomberman->y_goal > (int)bomberman->y){
		bomberman->direction = 0;
		bomberman->move_down = SDL_TRUE;
		bomberman->move_up = SDL_FALSE;
	}
	else if(bomberman->y_goal < (int)bomberman->y){
		bomberman->direction = 3;
		bomberman->move_up = SDL_TRUE;
		bomberman->move_down = SDL_FALSE;
	}


	update_position(board, bomberman);
	update_bomberman_animation(bomberman);
}

void set_new_goal(BOARD *board, BOMBERMAN *bomberman, int y, int x)
{
	if(!is_dangerous_area(board, y, x)){
		set_new_offense_goal(board, bomberman, y, x);
	} else {
		set_new_defense_goal(board, bomberman, y, x);
	}

}

void set_new_defense_goal(BOARD *board, BOMBERMAN *bomberman, int y, int x)
{
	int direction, next_x, next_y, cell_w, cell_h;
	char dir_x[] = {0,-1,1,0};
	char dir_y[] = {-1,0,0,1};

	cell_w = (WIDTH/board->c_size);
	cell_h = (HEIGHT/board->l_size);

	direction = get_direction_to_closest_safe_cell(board, y, x);

	next_x = get_next_val(x + dir_x[direction], board->c_size);
	next_y = get_next_val(y + dir_y[direction], board->l_size);

	bomberman->x_goal = next_x * cell_w + cell_w/2; // To get the center of the cell.
	bomberman->y_goal = next_y * cell_h + cell_h/2;
}

void set_new_offense_goal(BOARD *board, BOMBERMAN *bomberman, int y, int x)
{
	int direction, next_x, next_y, cell_w, cell_h;

	char dir_x[] = {0,-1,1,0};
	char dir_y[] = {-1,0,0,1};

	cell_w = (WIDTH/board->c_size);
	cell_h = (HEIGHT/board->l_size);
	direction = rand()%4;


	next_x = get_next_val(x + dir_x[direction], board->c_size);
	next_y = get_next_val(y + dir_y[direction], board->l_size);

	if(board->grid[next_y][next_x].type == GROUND && board->grid[next_y][next_x].bomb == NULL
			&& !is_dangerous_area(board, next_y, next_x) ){
		bomberman->x_goal = next_x * cell_w + cell_w/2; // To get the center of the cell.
		bomberman->y_goal = next_y * cell_h + cell_h/2;
	} else {
		bomberman->x_goal = 0;
		bomberman->y_goal = 0;
		bomberman->move_up = bomberman->move_down = bomberman->move_left = bomberman->move_right = SDL_FALSE;
	}

}

int get_direction_to_closest_safe_cell(BOARD *board, int y, int x)
{






	return 0;
}

// Check in the 4 direction for a bomb.
SDL_bool is_dangerous_area(BOARD *board, int y, int x)
{

	int i, j, counter;

	// Top
	i = y;
	j = x;
	counter = 0;

	while(counter < 20){
		counter++;
		i = (i - 1) < 0 ? (board->l_size - 1) : (i - 1);
		if(board->grid[i][j].type != GROUND){
			break;
		}
		if(board->grid[i][j].bomb != NULL && board->grid[i][j].bomb->power >= counter){
			return SDL_TRUE;
		}
	}

	// Bottom
	i = y;
	j = x;
	counter = 0;

	while(counter < 20){
		counter++;
		i = (i + 1) > (board->l_size - 1) ? 0 : (i + 1);
		if(board->grid[i][j].type != GROUND){
			break;
		}
		if(board->grid[i][j].bomb != NULL && board->grid[i][j].bomb->power >= counter){
			return SDL_TRUE;
		}
	}

	// Left
	i = y;
	j = x;
	counter = 0;

	while(counter < 20){
		counter++;
		j = (j - 1) < 0 ? (board->c_size - 1) : (j - 1);
		if(board->grid[i][j].type != GROUND){
			break;
		}
		if(board->grid[i][j].bomb != NULL && board->grid[i][j].bomb->power >= counter){
			return SDL_TRUE;
		}
	}

	// Right
	i = y;
	j = x;
	counter = 0;

	while(counter < 20){
		counter++;
		j = (j + 1) > (board->c_size - 1) ? 0 : (j + 1);
		if(board->grid[i][j].type != GROUND){
			break;
		}
		if(board->grid[i][j].bomb != NULL && board->grid[i][j].bomb->power >= counter){
			return SDL_TRUE;
		}
	}

	return SDL_FALSE;
}

