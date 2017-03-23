#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "bomb.h"
#include "bomberman.h"
#include "board.h"
#include "assets.h"
#include "graphic_param.h"

#define WIDTH 480
#define HEIGHT 480

#define FRAME_PER_WALK_ANIMATION 3
#define NB_FRAME 5
#define ANIMATION_SPEED 350

#define TIMER 150
#define TIMER_EXPLOSION 20

BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left)
{
	BOMB *bomb;
	bomb = malloc(sizeof(BOMB));
	bomb->x = x;
	bomb->y = y;
	bomb->bomberman_bomb_left = bomberman_bomb_left;
	bomb->has_explode = SDL_FALSE;
	bomb->timer = TIMER;
	bomb->power = power;
	bomb->sprite.x = 0;
	bomb->sprite.y = 26;
	bomb->sprite.w = 16;
	bomb->sprite.h = 17;
	bomb->up = bomb->down = bomb->right = bomb->left = 0;
	(*bomb->bomberman_bomb_left)--;
	return bomb;
}

void update_bomb_idle_animation(BOMB *bomb)
{
	Uint32 sprite = (SDL_GetTicks() / ANIMATION_SPEED) % FRAME_PER_WALK_ANIMATION;
	bomb->sprite.x = (sprite * bomb->sprite.w);
}


void update_bomb(BOARD *board, BOMB *bomb)
{
	if(!bomb->has_explode){
		update_bomb_idle_animation(bomb);
	} else {
		update_damages(board, bomb);
	}
	bomb->timer --;
}

SDL_bool render_bomb(BOARD *board, int start_y, int start_x, int current_y, int current_x, int counter_pos,
		DIRECTION direction, void *extra_parameters)
{
	GRAPHIC_PARAM *g_param = (GRAPHIC_PARAM *)(extra_parameters);

	BOMB * bomb = board->grid[start_y][start_x].bomb;


	// The center point where the texture will be rotated.
	SDL_Point center = {g_param->assets->explosion.w/2, g_param->assets->explosion.h/2};
	SDL_RendererFlip flip = SDL_FLIP_NONE; // the flip of the texture.

	float angle;
	int current_frame;
	SDL_bool needRenderEx = SDL_FALSE;

	current_frame = (int)(bomb->timer/(TIMER_EXPLOSION/NB_FRAME));

	angle = 0;
	g_param->draw_pos->y = current_y * (HEIGHT/board->l_size);
	g_param->draw_pos->x = current_x * (WIDTH/board->c_size);

	g_param->assets->explosion.x = 150 + ((NB_FRAME - current_frame - 1) * g_param->assets->explosion.w);
	g_param->assets->explosion.y = 41;

	// Top
	// Render here pour les bouts.
	switch(direction){
	case UP:
		if(counter_pos > bomb->up){
			return SDL_FALSE;
		}
		else if(counter_pos == bomb->up){
			needRenderEx = SDL_TRUE;
		}
		break;
	case LEFT:
		if(counter_pos > bomb->left){
			return SDL_FALSE;
		}
		else if(counter_pos == bomb->left){
			needRenderEx = SDL_TRUE;
		}
		angle = 90.0f;
		center.x = g_param->assets->explosion.w;
		center.y = g_param->assets->explosion.h;
		flip = SDL_FLIP_VERTICAL;
		break;
	case RIGHT:
		if(counter_pos > bomb->right){
			return SDL_FALSE;
		}
		else if(counter_pos == bomb->right){
			needRenderEx = SDL_TRUE;
		}
		angle = 90.0;
		center.x = g_param->assets->explosion.w/2;
		center.y = g_param->assets->explosion.h/2;
		break;
	case DOWN:
		if(counter_pos > bomb->down){
			return SDL_FALSE;
		}
		else if(counter_pos == bomb->down){
			needRenderEx = SDL_TRUE;
		}
		flip = SDL_FLIP_VERTICAL;
		break;
	case CENTER:
		g_param->assets->explosion.y = 57;
		break;
	default: break;
	}

	// Max power or on a bomb.
	if(counter_pos == bomb->power || (direction != CENTER && board->grid[current_y][current_x].bomb != NULL)){
		needRenderEx = SDL_TRUE;
	}

	if(needRenderEx){
		g_param->assets->explosion.y = 25;
		SDL_RenderCopyEx(g_param->renderer, g_param->assets->spritesheet, &g_param->assets->explosion , g_param->draw_pos,
				angle, &center, flip);
		return SDL_FALSE;
	} else {
		SDL_RenderCopyEx(g_param->renderer, g_param->assets->spritesheet, &g_param->assets->explosion , g_param->draw_pos,
				angle, &center, flip);
	}

	return SDL_TRUE;
}


void render_bombs(GRAPHIC_PARAM *g_param, BOARD *board)
{
	int i, j;

	BOMB *bomb;
	for (i = 0; i < board->l_size; i++){
		g_param->draw_pos->y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			g_param->draw_pos->x = j * (WIDTH/board->c_size);
			bomb = board->grid[i][j].bomb;
			// If there is a bomb on the cell.
			if(bomb != NULL){
				if(bomb->has_explode){
					function_around_pos(board, bomb->y, bomb->x, bomb->power, bomb->power, (void*)g_param, (*render_bomb));
				}
				else {
					SDL_RenderCopy(g_param->renderer, g_param->assets->spritesheet, &bomb->sprite, g_param->draw_pos);
				}
			}
		}
	}
}



int can_drop_bomb(BOARD *board, BOMBERMAN *bomberman)
{
	int x, y;
	x = from_pixel_to_grid_coord(board, bomberman->x, 1);
	y = from_pixel_to_grid_coord(board, bomberman->y, 0);

	return (bomberman->bomb_left > 0 && board->grid[y][x].bomb == NULL);
}

void drop_bomb(BOARD *board, BOMBERMAN *bomberman)
{
	int x, y;

	x = from_pixel_to_grid_coord(board, bomberman->x, 1);
	y = from_pixel_to_grid_coord(board, bomberman->y, 0);

	board->grid[y][x].bomb = init_bomb(x, y, bomberman->bomb_power, &bomberman->bomb_left);
}

void free_bomb(BOARD *board, BOMB *bomb)
{
	board->grid[bomb->y][bomb->x].bomb = NULL;
	free(bomb);
}

/**
 * Return false if the calling function (function_around_pos) should stop in the current direction.
 */
SDL_bool explode_cell(BOARD *board, int start_y, int start_x, int current_y, int current_x, int counter_pos,
		DIRECTION direction, void *extra_parameters)
{
	BOMB *bomb = board->grid[start_y][start_x].bomb;
	BOMB *possible_bomb = board->grid[current_y][current_x].bomb;
	ASSETS *assets = (ASSETS *)(extra_parameters);

	// Increase the size of the explosion for all directions.
	switch(direction){
	case UP: if(counter_pos > bomb->up) bomb->up = counter_pos; break;
	case DOWN: if(counter_pos > bomb->down) bomb->down = counter_pos; break;
	case RIGHT: if(counter_pos > bomb->right) bomb->right = counter_pos; break;
	case LEFT: if(counter_pos > bomb->left) bomb->left = counter_pos; break;
	default: break;
	}

	// Remove the bonus if it has one on this cell.
	if(board->grid[current_y][current_x].bonus != NULL){
		free(board->grid[current_y][current_x].bonus);
		board->grid[current_y][current_x].bonus = NULL;
	}

	// If the explosion is not on the center (first iteration the bomb blow on the center).
	// And we are on a bomb we stop.
	if(direction != CENTER && possible_bomb != NULL ){
		// Explode if it hadn't.
		if(!possible_bomb->has_explode){
			explode_around(board, possible_bomb, assets);
		}
		// And stop in that direction.
		return SDL_FALSE;
	}

	// Hit non ground type.
	if(board->grid[current_y][current_x].type != GROUND){
		if(board->grid[current_y][current_x].type == WALL_BREAKABLE){
			board->grid[current_y][current_x].type = GROUND;

			// Randomly pop a bonus (1/8).
			if(rand()%8 == 0){
				spawn_bonus(board, assets, current_y, current_x);
			}
		} else{ // Reduce the explosion size if it stop on a non breakable wall.
			switch(direction){
			case UP: bomb->up--;break;
			case DOWN:bomb->down--;	break;
			case RIGHT: bomb->right--; break;
			case LEFT: bomb->left--; break;
			default: break;
			}
		}
		return SDL_FALSE;
	}

	return SDL_TRUE;
}


void explode_around(BOARD *board, BOMB *bomb, ASSETS *assets)
{
	bomb->has_explode = SDL_TRUE;
	bomb->timer = TIMER_EXPLOSION;
	(*bomb->bomberman_bomb_left)++;

	function_around_pos(board, bomb->y, bomb->x, bomb->power, bomb->power, (void*)assets, (*explode_cell));
}

SDL_bool handle_damages(BOARD *board, int start_y, int start_x, int current_y, int current_x, int counter_pos,
		DIRECTION direction, void *extra_parameters)
{

	BOMB *bomb = board->grid[start_y][start_x].bomb;

	// Stop if it is too far.
	switch(direction){
	case UP: if(counter_pos > bomb->up) return SDL_FALSE; break;
	case DOWN: if(counter_pos > bomb->down) return SDL_FALSE; break;
	case RIGHT: if(counter_pos > bomb->right) return SDL_FALSE; break;
	case LEFT: if(counter_pos > bomb->left) return SDL_FALSE;	break;
	default: break;
	}

	// Kill any bomberman on the spot.
	if(board->grid[current_y][current_x].bomberman != NULL){
		if(!board->grid[current_y][current_x].bomberman->is_dead){
			board->grid[current_y][current_x].bomberman->is_dead = SDL_TRUE;
		}
	}

	return SDL_TRUE;
}

void update_damages(BOARD *board, BOMB *bomb)
{
	function_around_pos(board, bomb->y, bomb->x, bomb->power, bomb->power, NULL, (*handle_damages));
}


void function_around_pos(BOARD *board, int start_y, int start_x, unsigned int limit_y, unsigned int limit_x, void *parameters,
		SDL_bool (*function)(BOARD *board, int start_y, int start_x, int current_y, int current_x, int counter_pos,
				DIRECTION direction, void *extra_parameters))
{
	int x, y, counter_pos;

	counter_pos = 0;
	// Center
	(*function)(board, start_y, start_x, start_y, start_x, counter_pos, CENTER, parameters);

	// Top
	counter_pos = 1;
	for(y = start_y - 1; counter_pos <= limit_y; y--){
		y = (y < 0) ? (board->l_size - 1) : y;
		// Call the special function here that require to go in the 4 directions of the bomb such as:
		// render bomb, explode_around and handle_damages.
		// If the function should stop in that direction.
		if(! (*function)(board, start_y, start_x, y, start_x, counter_pos, UP, parameters)){
			break;
		}
		counter_pos++;
	}

	// Bottom
	counter_pos = 1;
	for(y = start_y + 1; counter_pos <= limit_y; y++){
		y = (y == board->l_size) ? 0 : y;
		if(! (*function)(board, start_y, start_x, y, start_x, counter_pos, DOWN, parameters)){
			break;
		}
		counter_pos++;
	}

	// Left
	counter_pos = 1;
	for(x = start_x - 1; counter_pos <= limit_x; x--){
		x = (x < 0) ? (board->c_size - 1): x;
		if(! (*function)(board, start_y, start_x, start_y, x, counter_pos, LEFT, parameters)){
			break;
		}
		counter_pos++;
	}

	// Right
	counter_pos = 1;
	for(x = start_x + 1; counter_pos <= limit_x; x++){
		x = (x == board->c_size) ? 0 : x;
		if(! (*function)(board, start_y, start_x, start_y, x, counter_pos, RIGHT, parameters)){
			break;
		}
		counter_pos++;
	}
}
