#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "bomb.h"
#include "bomberman.h"
#include "board.h"
#include "assets.h"

#define WIDTH 480
#define HEIGHT 480
#define FRAME_PER_ANIMATION 3
#define NB_FRAME 5
#define ANIMATION_SPEED 350
#define GROUND 0
#define WALL 1
#define WALL_BREAKABLE 2
#define TIMER 10000
#define TIMER_EXPLOSION 5000

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define CENTER 4

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
	(*bomb->bomberman_bomb_left)--;
	return bomb;
}

void update_bomb_idle_animation(BOMB *bomb)
{
	Uint32 sprite = (SDL_GetTicks() / ANIMATION_SPEED) % FRAME_PER_ANIMATION;
	bomb->sprite.x = (sprite * bomb->sprite.w);
}


void update_bomb(BOARD *board, BOMB *bomb)
{
	if(!bomb->has_explode){
		update_bomb_idle_animation(bomb);
	} else {
		update_damages(board, bomb);
	}
}

// Ca s'affiche mal car la case est mise a zero.
// On repasse dans la boucle alors que l'obstacle n'existe plus.
SDL_bool render_bomb(BOARD *board, ASSETS *assets, BOMB *bomb, int y, int x, int direction,
		SDL_Renderer *renderer, SDL_Rect *draw_pos)
{

	// The center point where the texture will be rotated.
	SDL_Point center = {assets->explosion.w/2, assets->explosion.h/2};
	SDL_RendererFlip flip = SDL_FLIP_NONE; // the flip of the texture.

	float angle;
	int val, counter_explo;
	val = (int)(bomb->timer/(TIMER_EXPLOSION/5));

	angle = 0;
	draw_pos->y = y * (HEIGHT/board->l_size);
	draw_pos->x = x * (WIDTH/board->c_size);

	assets->explosion.x = 150 + ((NB_FRAME - val - 1) * assets->explosion.w);
	assets->explosion.y = 41;

	// Top
	switch(direction){
	case UP:
		counter_explo = abs(y - bomb->y);
		if(counter_explo > bomb->up){
			return SDL_FALSE;
		}
		break;
	case LEFT:
		counter_explo = abs(x - bomb->x);
		if(counter_explo > bomb->left){
			return SDL_FALSE;
		}
		angle = 90.0f;
		center.x = assets->explosion.w;
		center.y = assets->explosion.h;
		flip = SDL_FLIP_VERTICAL;
		break;
	case RIGHT:
		counter_explo = abs(x - bomb->x);
		if(counter_explo > bomb->right){
			return SDL_FALSE;
		}
		angle = 90.0;
		center.x = assets->explosion.w/2;
		center.y = assets->explosion.h/2;
		flip = SDL_FLIP_NONE;
		break;
	case DOWN:
		counter_explo = abs(y - bomb->y);
		if(counter_explo > bomb->down){
			return SDL_FALSE;
		}
		flip = SDL_FLIP_VERTICAL;
		break;
	case CENTER:
		assets->explosion.y = 57;
		break;
	}

	if((direction != CENTER && board->grid[y][x].bomb != NULL) || board->grid[y][x].type != GROUND
			|| counter_explo == (bomb->power -1)){
		assets->explosion.y = 25;
		SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
				angle, &center, flip);
		return SDL_FALSE;
	} else {
		SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
				angle, &center, flip);
	}

	return SDL_TRUE;
}


void render_bombs(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos)
{
	int i, j;

	BOMB *bomb;
	for (i = 0; i < board->l_size; i++){
		draw_pos->y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			draw_pos->x = j * (WIDTH/board->c_size);

			// If there is a bomb on the cell.
			if(board->grid[i][j].bomb != NULL){
				bomb = board->grid[i][j].bomb;
				if(bomb->has_explode){
					function_around_bomb(board, bomb, assets, renderer, draw_pos, (*render_bomb));
				}
				else {
					SDL_RenderCopy(renderer, assets->spritesheet, &bomb->sprite, draw_pos);
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
 * Return false if the calling function (function around) should stop in the current direction.
 */
SDL_bool explode_cell(BOARD *board, ASSETS *assets,  BOMB *bomb, int y, int x, int direction,
		SDL_Renderer *renderer, SDL_Rect *draw_pos)
{
	BOMB *possible_bomb = board->grid[y][x].bomb;

	int height = abs(y - bomb->y);
	int width = abs(x - bomb->x);

	// Set the size of the explosion for all directions.
	switch(direction){
	case UP:
		if(bomb->up < height){
			bomb->up = height;
		}
		break;
	case DOWN:
		if(bomb->down < height){
			bomb->down = height;
		}
		break;
	case RIGHT:
		if(bomb->right < width){
			bomb->right= width;
		}
		break;
	case LEFT:
		if(bomb->left < width){
			bomb->left = width;
		}
		break;
	}

	// Remove the bonus if it has been hit by a bomb.
	if(board->grid[y][x].bonus != NULL){
		free(board->grid[y][x].bonus);
		board->grid[y][x].bonus = NULL;
	}

	if(possible_bomb != NULL ){
		if(!possible_bomb->has_explode){
			explode_around(board, possible_bomb, assets);
		}
		return SDL_FALSE;
	}
	if(board->grid[y][x].type != GROUND){
		if(board->grid[y][x].type == WALL_BREAKABLE){
			board->grid[y][x].type = GROUND;

			// Randomly pop a bonus.
			if(rand()%10 == 0){
				spawn_bonus(board, assets, x, y);
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

	function_around_bomb(board, bomb, assets, NULL, NULL, (*explode_cell));
}

SDL_bool handle_damages(BOARD *board, ASSETS *assets, BOMB *bomb, int y, int x, int direction,
		SDL_Renderer *renderer, SDL_Rect *draw_pos)
{

	int height = abs(y - bomb->y);
	int width = abs(x - bomb->x);

	switch(direction){
	case UP:
		if(bomb->up < height){
			return SDL_FALSE;
		}
		break;
	case DOWN:
		if(bomb->down < height){
			return SDL_FALSE;
		}
		break;
	case RIGHT:
		if(bomb->right < width){
			return SDL_FALSE;
		}
		break;
	case LEFT:
		if(bomb->left < width){
			return SDL_FALSE;
		}
		break;
	}


	if(board->grid[y][x].bomberman != NULL){
		board->grid[y][x].bomberman->is_dead = SDL_TRUE;
	}

	return SDL_TRUE;
}

void update_damages(BOARD *board, BOMB *bomb)
{
	function_around_bomb(board, bomb, NULL, NULL, NULL, (*handle_damages));
}




void function_around_bomb(BOARD *board, BOMB *bomb, ASSETS *assets, SDL_Renderer *renderer, SDL_Rect *draw_pos,
		SDL_bool (*function)(BOARD *board, ASSETS *assets, BOMB *bomb, int y, int x, int direction,
				SDL_Renderer *renderer, SDL_Rect *draw_pos))
{
	int x, y, counter_explo;

	// Center
	(*function)(board, assets, bomb, bomb->y, bomb->x, CENTER, renderer, draw_pos);

	// Top
	counter_explo = 1;
	for(y = bomb->y - 1; counter_explo <= bomb->power; y--){
		y = (y < 0) ? (board->l_size - 1) : y;
		counter_explo++;
		// Call the special function here that require to go in the 4 direction of the bomb such as.
		// render bomb or explode_around.

		// If the function should stop in that direction.
		if(! (*function)(board, assets, bomb, y, bomb->x, UP, renderer, draw_pos)){
			break;
		}
	}

	// Bottom
	counter_explo = 1;
	for(y = bomb->y + 1; counter_explo <= bomb->power; y++){
		y = (y == board->l_size) ? 0 : y;
		counter_explo++;
		if(! (*function)(board, assets, bomb, y, bomb->x, DOWN, renderer, draw_pos)){
			break;
		}
	}

	// Left
	counter_explo = 1;
	for(x = bomb->x - 1; counter_explo <= bomb->power; x--){
		x = (x < 0) ? (board->c_size - 1): x;
		counter_explo++;
		if(! (*function)(board, assets, bomb, bomb->y, x, LEFT, renderer, draw_pos)){
			break;
		}
	}

	// Right
	counter_explo = 1;
	for(x = bomb->x + 1; counter_explo <= bomb->power; x++){
		x = (x == board->c_size) ? 0 : x;
		counter_explo++;
		if(! (*function)(board, assets, bomb, bomb->y, x, RIGHT, renderer, draw_pos)){
			break;
		}
	}

}
