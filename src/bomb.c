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

BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left)
{
	BOMB *bomb;
	bomb = malloc(sizeof(BOMB));
	bomb->x = x;
	bomb->y = y;
	bomb->bomberman_bomb_left = bomberman_bomb_left;
	bomb->has_explode = SDL_FALSE;
	bomb->timer = 10000;
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
		handle_damages(board, bomb);
	}
}


void handle_damages(BOARD *board, BOMB *bomb)
{
	int x, y, counter_explo;

	if(board->grid[bomb->y][bomb->x].bomberman != NULL){
		//Kill him
		printf("dead\n");
	}

	// Top
	counter_explo = 1;
	for(y = bomb->y - 1; counter_explo <= bomb->power; y--){

		y = (y < 0) ? (board->l_size - 1): y;

		if(board->grid[y][bomb->x].bomberman != NULL){
			// kill him
			printf("dead\n");
		}
		// We stop on a bomb or non ground type.
		if(board->grid[y][bomb->x].type != GROUND || board->grid[y][bomb->x].bomb != NULL){
			break;
		}
		counter_explo++;
	}

	// Bottom
	counter_explo = 1;
	for(y = bomb->y + 1; counter_explo <= bomb->power; y++){

		y = (y == board->l_size) ? 0 : y;

		if(board->grid[y][bomb->x].bomberman != NULL){
			// kill him
			printf("dead\n");
		}
		if(board->grid[y][bomb->x].type != GROUND || board->grid[y][bomb->x].bomb != NULL){
			break;
		}
		counter_explo++;
	}

	// Left
	counter_explo = 1;
	for(x = bomb->x - 1; counter_explo <= bomb->power; x--){

		x = (x < 0) ? (board->c_size - 1): x;

		if(board->grid[bomb->y][x].bomberman != NULL){
			// kill him
			printf("dead\n");
		}
		if(board->grid[bomb->y][x].type != GROUND || board->grid[bomb->y][x].bomb != NULL){
			break;
		}
		counter_explo++;
	}

	// Right
	counter_explo = 1;
	for(x = bomb->x + 1; counter_explo <= bomb->power; x++){

		x = (x == board->c_size) ? 0 : x;

		if(board->grid[bomb->y][x].bomberman != NULL){
			// kill him
			printf("dead\n");
		}
		if(board->grid[bomb->y][x].type != GROUND || board->grid[bomb->y][x].bomb != NULL){
			break;
		}
		counter_explo++;
	}
	fflush(stdout);
}


void render_bombs(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos)
{
	int i, j, x, y, counter_explo;
	float angle;
	// the center where the texture will be rotated.
	SDL_Point center = {assets->explosion.w/2, assets->explosion.h/2};
	SDL_RendererFlip flip = SDL_FLIP_NONE; // the flip of the texture.

	BOMB *bomb;
	for (i = 0; i < board->l_size; i++){
		draw_pos->y = i * (HEIGHT/board->l_size);
		for (j = 0; j < board->c_size; j++){
			draw_pos->x = j * (WIDTH/board->c_size);
			if(board->grid[i][j].bomb != NULL){
				bomb = board->grid[i][j].bomb;

				if(board->grid[i][j].bomb->has_explode){

					/*
					printf("bomb_timer = %d\n", bomb->timer);
					printf("val = %d\n", val);
					fflush(stdout);
					 */

					int val = (int)(bomb->timer/200);
					assets->explosion.x = 150 + ( (NB_FRAME - val - 1) * assets->explosion.w);

					// Center
					assets->explosion.y = 57;
					SDL_RenderCopy(renderer, assets->spritesheet, &assets->explosion, draw_pos);

					// Top
					angle = 0;
					assets->explosion.y = 41;

					counter_explo = 1;
					for(y = bomb->y - 1; counter_explo <= bomb->power; y--){

						y = (y < 0) ? (board->l_size - 1): y;
						if(board->grid[y][bomb->x].bomb != NULL){
							break;
						}
						draw_pos->y = y * (HEIGHT/board->l_size);

						if(board->grid[y][bomb->x].type != GROUND ||counter_explo == bomb->power){
							assets->explosion.y = 25;
							SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
									angle, &center, flip);
							break;
						}
						counter_explo++;
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}

					// Bottom
					counter_explo = 1;
					assets->explosion.y = 41;
					flip = SDL_FLIP_VERTICAL;
					for(y = bomb->y + 1; counter_explo <= bomb->power; y++){

						y = (y == board->l_size) ? 0 : y;
						if(board->grid[y][bomb->x].bomb != NULL){
							break;
						}
						draw_pos->y = y * (HEIGHT/board->l_size);
						if(board->grid[y][bomb->x].type != GROUND || counter_explo == bomb->power){
							assets->explosion.y = 25;
							SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
									angle, &center, flip);
							break;
						}
						counter_explo++;
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}

					draw_pos->y = i * (HEIGHT/board->l_size);

					// Left
					counter_explo = 1;
					center.x = assets->explosion.w;
					center.y = assets->explosion.h;
					angle = 90.0f;
					assets->explosion.y = 41;
					for(x = bomb->x - 1; counter_explo <= bomb->power; x--){

						x = (x < 0) ? (board->c_size - 1): x;
						if(board->grid[bomb->y][x].bomb != NULL){
							break;
						}
						draw_pos->x = x * (WIDTH/board->c_size);
						if(board->grid[bomb->y][x].type != GROUND || counter_explo == bomb->power){
							assets->explosion.y = 25;
							SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
									angle, &center, flip);
							break;
						}
						counter_explo++;
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}

					// Right
					counter_explo = 1;
					angle = 90.0;
					assets->explosion.y = 41;
					center.x = assets->explosion.w/2;
					center.y = assets->explosion.h/2;
					flip = SDL_FLIP_NONE;
					for(x = bomb->x + 1; counter_explo <= bomb->power; x++){

						x = (x == board->c_size) ? 0 : x;
						if(board->grid[bomb->y][x].bomb != NULL){
							break;
						}
						draw_pos->x = x * (WIDTH/board->c_size);
						if(board->grid[bomb->y][x].type != GROUND || counter_explo == bomb->power){
							assets->explosion.y = 25;
							SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
									angle, &center, flip);
							break;
						}
						counter_explo++;
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}
				}
				else {
					SDL_RenderCopy(renderer, assets->spritesheet, &bomb->sprite, draw_pos);
				}

			}
		}
	}
}

void explode(BOARD *board, BOMB *bomb)
{
	int x, y, counter_explo;
	bomb->has_explode = SDL_TRUE;

	// Top
	counter_explo = 1;
	for(y = bomb->y - 1; counter_explo <= bomb->power; y--){

		y = (y < 0) ? (board->l_size - 1): y;
		if(board->grid[y][bomb->x].type != GROUND){
			if(board->grid[y][bomb->x].type == WALL_BREAKABLE){
				board->grid[y][bomb->x].type = GROUND;
			}
			break;
		}
		else if(board->grid[y][bomb->x].bomb != NULL){
			if(!board->grid[y][bomb->x].bomb->has_explode){
				explode(board, board->grid[y][bomb->x].bomb);
			}
			break;
		}
		counter_explo++;
	}

	// Bottom
	counter_explo = 1;
	for(y = bomb->y + 1; counter_explo <= bomb->power; y++){

		y = (y == board->l_size) ? 0 : y;
		if(board->grid[y][bomb->x].type != GROUND){
			if(board->grid[y][bomb->x].type == WALL_BREAKABLE){
				board->grid[y][bomb->x].type = GROUND;
			}
			break;
		}
		else if(board->grid[y][bomb->x].bomb != NULL){
			if(!board->grid[y][bomb->x].bomb->has_explode){
				explode(board, board->grid[y][bomb->x].bomb);
			}
			break;
		}
		counter_explo++;
	}

	// Left
	counter_explo = 1;
	for(x = bomb->x - 1; counter_explo <= bomb->power; x--){

		x = (x < 0) ? (board->c_size - 1): x;
		if(board->grid[bomb->y][x].type != GROUND){
			if(board->grid[bomb->y][x].type == WALL_BREAKABLE){
				board->grid[bomb->y][x].type = GROUND;
			}
			break;
		}
		else if(board->grid[bomb->y][x].bomb != NULL){
			if(!board->grid[bomb->y][x].bomb->has_explode){
				explode(board, board->grid[bomb->y][x].bomb);
			}
			break;
		}
		counter_explo++;
	}

	// Right
	counter_explo = 1;
	for(x = bomb->x + 1; counter_explo <= bomb->power; x++){

		x = (x == board->c_size) ? 0 : x;
		if(board->grid[bomb->y][x].type != GROUND){
			if(board->grid[bomb->y][x].type == WALL_BREAKABLE){
				board->grid[bomb->y][x].type = GROUND;
			}
			break;
		}
		else if(board->grid[bomb->y][x].bomb != NULL){
			if(!board->grid[bomb->y][x].bomb->has_explode){
				explode(board, board->grid[bomb->y][x].bomb);
			}
			break;
		}
		counter_explo++;
	}

	bomb->timer = 1000;
	(*bomb->bomberman_bomb_left)++;
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
