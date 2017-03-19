/*
 * ai.c
 *
 *  Created on: 19 déc. 2016
 *  Author: Jean-Hugo
 *  Inspired by http://www.tigen.org/jyaif/Article/Bomberdude_ai.htm
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

SDL_bool wrapping;

void update_ai_bomberman(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction)
{
	int x, y, cell_w, cell_h;

	x = from_pixel_to_grid_coord(board, bomberman->x, 1);
	y = from_pixel_to_grid_coord(board, bomberman->y, 0);

	cell_h = (HEIGHT/board->l_size);
	cell_w = (WIDTH/board->c_size);

	// If his goal distance is more than a cell width/height then it had wrapped.
	if(abs(bomberman->y_goal - bomberman->y) > cell_h || abs(bomberman->x_goal - bomberman->x) > cell_w){
		wrapping = SDL_TRUE;
	} else {
		wrapping = SDL_FALSE;
	}

	//printf("x_g=%d, y_g=%d\n", bomberman->x_goal/board->l_size, bomberman->y_goal/board->c_size);
	//printf("x_r=%d, y_r=%d\n", (int)bomberman->x/board->l_size, (int)bomberman->y/board->c_size);
	//printf("Is dangerous area ? %d\n", is_dangerous_area(board, y, x));

	// When the bot has no goal.
	if((bomberman->y_goal == -1 && bomberman->x_goal == -1) ||
			// When the bot reached his goal.
			(bomberman->y_goal == (int) bomberman->y && bomberman->x_goal == (int) bomberman->x)){
		set_new_goal(board, bomberman, grid_iteration, grid_direction, y, x);
	}

	else if(bomberman->x_goal > (int)bomberman->x){
		bomberman->direction = wrapping ? 1 : 2;
		bomberman->move_right =  wrapping ? SDL_FALSE : SDL_TRUE;
		bomberman->move_left =  wrapping ? SDL_TRUE: SDL_FALSE;
	}
	else if(bomberman->x_goal < (int)bomberman->x){
		bomberman->direction = wrapping ? 2 : 1;
		bomberman->move_left =  wrapping ? SDL_FALSE : SDL_TRUE;
		bomberman->move_right =  wrapping ? SDL_TRUE : SDL_FALSE;
	}
	else if(bomberman->y_goal > (int)bomberman->y){
		bomberman->direction = wrapping ? 3 : 0;
		bomberman->move_down = wrapping ? SDL_FALSE : SDL_TRUE;
		bomberman->move_up = wrapping ? SDL_TRUE : SDL_FALSE;
	}
	else if(bomberman->y_goal < (int)bomberman->y){
		bomberman->direction = wrapping ? 0 : 3;
		bomberman->move_up = wrapping ? SDL_FALSE : SDL_TRUE;
		bomberman->move_down = wrapping ? SDL_TRUE : SDL_FALSE;
	}

	update_bomberman(board, bomberman);
}

void set_new_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction, int y, int x)
{
	if(!is_dangerous_area(board, y, x)){
		set_new_offense_goal(board, bomberman, y, x);
	} else {
		set_new_defense_goal(board, bomberman, grid_iteration, grid_direction, y, x);
	}

}

void set_new_defense_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction,
		int y, int x)
{
	int direction, next_x, next_y, cell_w, cell_h;
	char dir_x[] = {0,-1,1,0};
	char dir_y[] = {1,0,0,-1};

	cell_w = (WIDTH/board->c_size);
	cell_h = (HEIGHT/board->l_size);

	direction = get_direction_to_closest_safe_cell(board, grid_iteration, grid_direction, y, x);

	if(direction >= 0){

		next_x = get_next_val(x + dir_x[direction], board->c_size);
		next_y = get_next_val(y + dir_y[direction], board->l_size);

		bomberman->x_goal = next_x * cell_w + cell_w/2; // To get the center of the cell.
		bomberman->y_goal = next_y * cell_h + cell_h/2;
	} else {
		bomberman->x_goal = -1;
		bomberman->y_goal = -1;
		bomberman->move_up = bomberman->move_down = bomberman->move_left = bomberman->move_right = SDL_FALSE;
	}
}

void set_new_offense_goal(BOARD *board, BOMBERMAN *bomberman, int y, int x)
{
	int direction, next_x, next_y, cell_w, cell_h;

	char dir_x[] = {0,-1,1,0};
	char dir_y[] = {1,0,0,-1};

	cell_w = (WIDTH/board->c_size);
	cell_h = (HEIGHT/board->l_size);
	direction = (int) (rand()%100);

	// It try not to go back in the previous direction.
	switch(bomberman->direction){
	case 0:// Opposite direction is 3
		direction = direction < 60 ? 0 : direction < 78 ? 1 : direction < 98 ? 2 : 3;
		break;
	case 1:// Opposite direction is 2
		direction = direction < 60 ? 1 : direction < 78 ? 0 : direction < 98 ? 3 : 2;
		break;
	case 2:// Opposite direction is 1
		direction = direction < 60 ? 2 : direction < 78 ? 0 : direction < 98 ? 3 : 1;
		break;
	case 3:// Opposite direction is 0
		direction = direction < 60 ? 3 : direction < 78 ? 1 : direction < 98 ? 3 : 0;
		break;
	}


	next_x = get_next_val(x + dir_x[direction], board->c_size);
	next_y = get_next_val(y + dir_y[direction], board->l_size);

	if(board->grid[next_y][next_x].type == GROUND && !is_dangerous_area(board, next_y, next_x) ){
		bomberman->x_goal = next_x * cell_w + cell_w/2; // To get the center of the cell.
		bomberman->y_goal = next_y * cell_h + cell_h/2;
		try_to_drop_bomb(board, bomberman);
	} else {
		bomberman->x_goal = -1;
		bomberman->y_goal = -1;
		bomberman->move_up = bomberman->move_down = bomberman->move_left = bomberman->move_right = SDL_FALSE;
	}

}

void mark_around_position(BOARD *board, int **grid_iteration, int **grid_direction, int y, int x,
		int iteration)
{
	int next_y, next_x;

	// DOWN
	next_y = get_next_val(y + 1, board->l_size);
	// La case est accessible et elle n'a pas été marquée
	if(grid_iteration[next_y][x] < 0 && board->grid[next_y][x].type == GROUND && board->grid[next_y][x].bomb == NULL){
		// On marque l'iteration
		grid_iteration[next_y][x] = iteration;
		// On marque la même direction que la case adjacente si elle existe. Sinon c'est la premiere iteration.
		grid_direction[next_y][x] = grid_direction[y][x] >= 0 ? grid_direction[y][x] : 0;
	}

	// LEFT
	next_x = get_next_val(x - 1, board->c_size);
	if(grid_iteration[y][next_x] < 0 && board->grid[y][next_x].type == GROUND && board->grid[y][next_x].bomb == NULL){
		grid_iteration[y][next_x] = iteration;
		grid_direction[y][next_x] = grid_direction[y][x] >= 0 ? grid_direction[y][x] : 1;
	}

	// RIGHT
	next_x = get_next_val(x + 1, board->c_size);
	if(grid_iteration[y][next_x] < 0 && board->grid[y][next_x].type == GROUND && board->grid[y][next_x].bomb == NULL){
		grid_iteration[y][next_x] = iteration;
		grid_direction[y][next_x] = grid_direction[y][x] >= 0 ? grid_direction[y][x] : 2;
	}

	// UP
	next_y = get_next_val(y - 1, board->l_size);
	if(grid_iteration[next_y][x] < 0 && board->grid[next_y][x].type == GROUND && board->grid[next_y][x].bomb == NULL ){
		grid_iteration[next_y][x] = iteration;
		grid_direction[next_y][x] = grid_direction[y][x] >= 0 ? grid_direction[y][x] : 3;
	}

}

int get_direction_to_closest_safe_cell(BOARD *board, int **grid_iteration, int **grid_direction, int y, int x)
{
	int i, j, iteration;

	// Reset the array and mark the initial position.
	for(i = 0; i < board->l_size; i++){
		for(j = 0; j < board->c_size; j++){
			grid_iteration[i][j] = -1;
			grid_direction[i][j] = -1;
		}
	}
	grid_iteration[y][x] = 0;

	// On parcours le tableau des iterations (5 fois)
	for(iteration = 1; iteration <= 5; iteration++){

		for(i = 0; i < board->l_size; i++){
			for(j = 0; j < board->c_size; j++){

				// On marque les cases adjacentes à l'iteration precedente.
				if(grid_iteration[i][j] == (iteration - 1)){
					mark_around_position(board, grid_iteration, grid_direction, i, j, iteration);
				}
			}
		}

		// Check if one find a safe area.
		for(i = 0; i < board->l_size; i++){
			for(j = 0; j < board->c_size; j++){
				// If the position is marked and if it is safe.
				if(grid_iteration[i][j] > 0 && !is_dangerous_area(board, i, j)){
					return grid_direction[i][j];
				}
			}
		}
	}
	// Haven't find any path (very unlikely to happen).
	return -1;
}


// Check in the 4 directions for a bomb.
SDL_bool is_dangerous_area(BOARD *board, int y, int x)
{

	int i, j, counter;
	i = y;
	j = x;
	counter = 0;

	// On the point.
	if(board->grid[i][j].bomb != NULL){
		return SDL_TRUE;
	}

	// Top
	while(counter < board->l_size){
		counter++;
		i = get_next_val(i - 1, board->l_size);
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

	while(counter < board->l_size){
		counter++;
		i = get_next_val(i + 1, board->l_size);
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

	while(counter < board->c_size){
		counter++;
		j = get_next_val(j - 1, board->c_size);
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

	while(counter < board->c_size){
		counter++;
		j = get_next_val(j + 1, board->c_size);
		if(board->grid[i][j].type != GROUND){
			break;
		}
		if(board->grid[i][j].bomb != NULL && board->grid[i][j].bomb->power >= counter){
			return SDL_TRUE;
		}
	}

	return SDL_FALSE;
}


void try_to_drop_bomb(BOARD *board, BOMBERMAN *bomberman){

	// On each offensive goal, it has 1/8 chances to drop a bomb.
	int alea = rand()%8;
	int x,y;

	if(alea == 0 && can_drop_bomb(board, bomberman)){

		// Test if the area will still be safe after it dropped the bomb.
		y = from_pixel_to_grid_coord(board, bomberman->y, 0);
		x = from_pixel_to_grid_coord(board, bomberman->x, 1);
		if(is_around_safe(board, y, x) == SDL_TRUE){
			drop_bomb(board, bomberman);
		}
	}
}

/**
 * Need a rework to check for a path.
 */
SDL_bool is_around_safe(BOARD *board, int y, int x)
{
	int next_y, next_x;
	SDL_bool safe = SDL_FALSE;

	next_y = get_next_val(y + 1, board->l_size);
	next_x = get_next_val(x + 1, board->c_size);
	if(!is_dangerous_area(board, next_y, next_x)){
		safe = SDL_TRUE;
	}

	next_y = get_next_val(y - 1, board->l_size);
	next_x = get_next_val(x + 1, board->c_size);
	if(!is_dangerous_area(board, next_y, next_x)){
		safe = SDL_TRUE;
	}

	next_y = get_next_val(y + 1, board->l_size);
	next_x = get_next_val(x - 1, board->c_size);
	if(!is_dangerous_area(board, next_y, next_x)){
		safe = SDL_TRUE;
	}

	next_y = get_next_val(y - 1, board->l_size);
	next_x = get_next_val(x - 1, board->c_size);
	if(!is_dangerous_area(board, next_y, next_x)){
		safe = SDL_TRUE;
	}

	return safe;
}
