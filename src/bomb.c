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

#define TIMER 10000
#define TIMER_EXPLOSION 1000

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
	bomb->timer --;
}

SDL_bool render_bomb(BOARD *board, ASSETS *assets, BOMB *bomb, int counter_explo, int y, int x, DIRECTION direction,
		SDL_Renderer *renderer, SDL_Rect *draw_pos)
{
	// The center point where the texture will be rotated.
	SDL_Point center = {assets->explosion.w/2, assets->explosion.h/2};
	SDL_RendererFlip flip = SDL_FLIP_NONE; // the flip of the texture.

	float angle;
	int val;
	SDL_bool needRenderEx = SDL_FALSE;

	val = (int)(bomb->timer/(TIMER_EXPLOSION/5));

	angle = 0;
	draw_pos->y = y * (HEIGHT/board->l_size);
	draw_pos->x = x * (WIDTH/board->c_size);

	assets->explosion.x = 150 + ((NB_FRAME - val - 1) * assets->explosion.w);
	assets->explosion.y = 41;

	// Top
	// Render here pour les bouts.
	switch(direction){
	case UP:
		if(counter_explo > bomb->up){
			return SDL_FALSE;
		}
		else if(counter_explo == bomb->up){
			needRenderEx = SDL_TRUE;
		}
		break;
	case LEFT:
		if(counter_explo > bomb->left){
			return SDL_FALSE;
		}
		else if(counter_explo == bomb->left){
			needRenderEx = SDL_TRUE;
		}
		angle = 90.0f;
		center.x = assets->explosion.w;
		center.y = assets->explosion.h;
		flip = SDL_FLIP_VERTICAL;
		break;
	case RIGHT:
		if(counter_explo > bomb->right){
			return SDL_FALSE;
		}
		else if(counter_explo == bomb->right){
			needRenderEx = SDL_TRUE;
		}
		angle = 90.0;
		center.x = assets->explosion.w/2;
		center.y = assets->explosion.h/2;
		flip = SDL_FLIP_NONE;
		break;
	case DOWN:
		if(counter_explo > bomb->down){
			return SDL_FALSE;
		}
		else if(counter_explo == bomb->down){
			needRenderEx = SDL_TRUE;
		}
		flip = SDL_FLIP_VERTICAL;
		break;
	case CENTER:

		assets->explosion.y = 57;
		break;
	}

	// Max power or on a bomb.
	if(counter_explo == bomb->power || (direction != CENTER && board->grid[y][x].bomb != NULL)){
		needRenderEx = SDL_TRUE;
	}

	if(needRenderEx){
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
			bomb = board->grid[i][j].bomb;

			// If there is a bomb on the cell.
			if(bomb != NULL){
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
SDL_bool explode_cell(BOARD *board, ASSETS *assets,  BOMB *bomb, int counter_explo, int y, int x, DIRECTION direction,
		SDL_Renderer *renderer, SDL_Rect *draw_pos)
{
	BOMB *possible_bomb = board->grid[y][x].bomb;

	// Increase the size of the explosion for all directions.
	switch(direction){
	case UP: if(counter_explo > bomb->up) bomb->up = counter_explo; break;
	case DOWN: if(counter_explo > bomb->down) bomb->down = counter_explo; break;
	case RIGHT: if(counter_explo > bomb->right) bomb->right = counter_explo; break;
	case LEFT: if(counter_explo > bomb->left) bomb->left = counter_explo; break;
	default: break;
	}


	// Remove the bonus if it has one on this cell.
	if(board->grid[y][x].bonus != NULL){
		free(board->grid[y][x].bonus);
		board->grid[y][x].bonus = NULL;
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
	if(board->grid[y][x].type != GROUND){
		if(board->grid[y][x].type == WALL_BREAKABLE){
			board->grid[y][x].type = GROUND;

			// Randomly pop a bonus (1/10).
			if(rand()%10 == 0){
				spawn_bonus(board, assets, x, y);
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
		/* Thanks to this output I got the debug I should have init my bomb->direction values...
		printf("Bomb[%d][%d], on cell[%d][%d]counter_explo=%d, "
				, bomb->y, bomb->x, y, x, counter_explo);

		switch(direction){
		case UP: printf("dir=UP, bomb->up=%d\n", bomb->up); break;
		case DOWN: printf("dir=DOWN, bomb->down=%d\n", bomb->down); break;
		case RIGHT: printf("dir=RIGHT, bomb->right=%d\n", bomb->right); break;
		case LEFT: printf("dir=LEFT, bomb->left=%d\n", bomb->left); break;
		default: break;
		fflush(stdout);
		}*/
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

SDL_bool handle_damages(BOARD *board, ASSETS *assets, BOMB *bomb, int counter_explo, int y, int x, DIRECTION direction,
		SDL_Renderer *renderer, SDL_Rect *draw_pos)
{

	// Stop if it is too far.
	switch(direction){
	case UP: if(counter_explo > bomb->up) return SDL_FALSE; break;
	case DOWN: if(counter_explo > bomb->down) return SDL_FALSE; break;
	case RIGHT: if(counter_explo > bomb->right) return SDL_FALSE; break;
	case LEFT: if(counter_explo > bomb->left) return SDL_FALSE;	break;
	default: break;
	}

	// Kill any bomberman on the spot.
	if(board->grid[y][x].bomberman != NULL){
		if(!board->grid[y][x].bomberman->is_dead){
			board->grid[y][x].bomberman->is_dead = SDL_TRUE;
		}
	}

	return SDL_TRUE;
}

void update_damages(BOARD *board, BOMB *bomb)
{
	function_around_bomb(board, bomb, NULL, NULL, NULL, (*handle_damages));
}




void function_around_bomb(BOARD *board, BOMB *bomb, ASSETS *assets, SDL_Renderer *renderer, SDL_Rect *draw_pos,
		SDL_bool (*function)(BOARD *board, ASSETS *assets, BOMB *bomb, int counter_explo, int y, int x,
				DIRECTION direction, SDL_Renderer *renderer, SDL_Rect *draw_pos))
{
	int x, y, counter_explo;

	counter_explo = 0;
	// Center
	(*function)(board, assets, bomb, counter_explo, bomb->y, bomb->x, CENTER, renderer, draw_pos);

	// Top
	counter_explo = 1;
	for(y = bomb->y - 1; counter_explo <= bomb->power; y--){
		y = (y < 0) ? (board->l_size - 1) : y;
		// Call the special function here that require to go in the 4 directions of the bomb such as:
		// render bomb, explode_around and handle_damages.
		// If the function should stop in that direction.
		if(! (*function)(board, assets, bomb, counter_explo, y, bomb->x, UP, renderer, draw_pos)){
			break;
		}
		counter_explo++;
	}

	// Bottom
	counter_explo = 1;
	for(y = bomb->y + 1; counter_explo <= bomb->power; y++){
		y = (y == board->l_size) ? 0 : y;
		if(! (*function)(board, assets, bomb, counter_explo, y, bomb->x, DOWN, renderer, draw_pos)){
			break;
		}
		counter_explo++;
	}

	// Left
	counter_explo = 1;
	for(x = bomb->x - 1; counter_explo <= bomb->power; x--){
		x = (x < 0) ? (board->c_size - 1): x;
		if(! (*function)(board, assets, bomb, counter_explo, bomb->y, x, LEFT, renderer, draw_pos)){
			break;
		}
		counter_explo++;
	}

	// Right
	counter_explo = 1;
	for(x = bomb->x + 1; counter_explo <= bomb->power; x++){
		x = (x == board->c_size) ? 0 : x;
		if(! (*function)(board, assets, bomb, counter_explo, bomb->y, x, RIGHT, renderer, draw_pos)){
			break;
		}
		counter_explo++;
	}

}
