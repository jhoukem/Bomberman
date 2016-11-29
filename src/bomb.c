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
#define SPRITE_WIDTH 16
#define ANIMATION_SPEED 350

BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left)
{
	BOMB *bomb;
	bomb = malloc(sizeof(BOMB));
	bomb->x = x;
	bomb->y = y;
	bomb->has_explode = false;
	bomb->bomberman_bomb_left = bomberman_bomb_left;
	bomb->timer = 10000;
	bomb->power = power;
	bomb->sprite.x = 0;
	bomb->sprite.y = 26;
	bomb->sprite.w = 16;
	bomb->sprite.h = 17;
	(*bomb->bomberman_bomb_left)--;
	return bomb;
}

void update_bomb_animation(BOMB *bomb)
{
	Uint32 sprite = (SDL_GetTicks() / ANIMATION_SPEED) % FRAME_PER_ANIMATION;
	bomb->sprite.x = (sprite * SPRITE_WIDTH);
}


void update_bomb(BOARD *board, BOMB *bomb)
{
	update_bomb_animation(bomb);
	// Other things like kill the bomberman...
}

void render_bombs(BOARD *board, SDL_Renderer *renderer, ASSETS *assets, SDL_Rect *draw_pos)
{
	int i, j, x, y;
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

					printf("bomb_timer = %d\n", bomb->timer);
					int val = (int)(bomb->timer/200);
					printf("val = %d\n", val);
					fflush(stdout);

					assets->explosion.x = 150 + ( (NB_FRAME - val - 1) * assets->explosion.w);
					printf("assets explo.x = %d\n", assets->explosion.x);

					// Center
					assets->explosion.y = 57;
					SDL_RenderCopy(renderer, assets->spritesheet, &assets->explosion, draw_pos);

					// Top
					angle = 0;
					assets->explosion.y = 41;
					for(y = bomb->y - 1; y >= 0 && (-(y - bomb->y)) <= bomb->power; y--){
						if(board->grid[y][bomb->x].bomb != NULL){
							break;
						}
						draw_pos->y = y * (HEIGHT/board->l_size);

						if(y == 0 || (-(y - bomb->y)) == bomb->power){
							assets->explosion.y = 25;
						}
						//SDL_RenderCopy(renderer, assets->spritesheet, &assets->explosion, draw_pos);
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}

					// Bottom
					assets->explosion.y = 41;
					flip = SDL_FLIP_VERTICAL;
					for(y = bomb->y + 1; y < board->l_size && (y - bomb->y) <= bomb->power; y++){
						if(board->grid[y][bomb->x].bomb != NULL){
							break;
						}
						draw_pos->y = y * (HEIGHT/board->l_size);
						if(y == (board->l_size - 1) || (y - bomb->y) == bomb->power){
							assets->explosion.y = 25;
						}
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}

					draw_pos->y = i * (HEIGHT/board->l_size);

					// Left
					center.x = assets->explosion.w;
					center.y = assets->explosion.h;
					angle = 90.0f;
					assets->explosion.y = 41;
					for(x = bomb->x - 1; x >= 0 && (-(x - bomb->x)) <= bomb->power; x--){
						if(board->grid[bomb->y][x].bomb != NULL){
							break;
						}
						draw_pos->x = x * (WIDTH/board->c_size);
						if(x == 0 || (-(x - bomb->x)) == bomb->power){
							assets->explosion.y = 25;
						}
						SDL_RenderCopyEx(renderer, assets->spritesheet, &assets->explosion , draw_pos,
								angle, &center, flip);
					}

					// Right
					angle = 90.0;
					assets->explosion.y = 41;
					center.x = assets->explosion.w/2;
					center.y = assets->explosion.h/2;
					flip = SDL_FLIP_NONE;
					for(x = bomb->x + 1; x < board->c_size && (x - bomb->x) <= bomb->power; x++){
						if(board->grid[bomb->y][x].bomb != NULL){
							break;
						}
						draw_pos->x = x * (WIDTH/board->c_size);
						if(x == (board->c_size - 1) || (x - bomb->x) == bomb->power){
							assets->explosion.y = 25;
						}
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
	int x, y;
	bomb->has_explode = true;
	// Top
	for(y = bomb->y - 1; y >= 0 && (-(y - bomb->y)) <= bomb->power; y--){
		if(board->grid[y][bomb->x].bomb != NULL && !board->grid[y][bomb->x].bomb->has_explode){
			explode(board, board->grid[y][bomb->x].bomb);
			break;
		}
	}
	// Bottom
	for(y = bomb->y + 1; y < board->l_size && (y - bomb->y) <= bomb->power; y++){
		if(board->grid[y][bomb->x].bomb != NULL && !board->grid[y][bomb->x].bomb->has_explode){
			explode(board, board->grid[y][bomb->x].bomb);
			break;
		}
	}
	// Left
	for(x = bomb->x - 1; x >= 0 && (-(x - bomb->x)) <= bomb->power; x--){
		if(board->grid[bomb->y][x].bomb != NULL && !board->grid[bomb->y][x].bomb->has_explode){
			explode(board, board->grid[bomb->y][x].bomb);
			break;
		}
	}
	// Right
	for(x = bomb->x + 1; x < board->c_size && (x - bomb->x) <= bomb->power; x++){
		if(board->grid[bomb->y][x].bomb != NULL && !board->grid[bomb->y][x].bomb->has_explode){
			explode(board, board->grid[bomb->y][x].bomb);
			break;
		}
	}

	bomb->timer = 1000;
	(*bomb->bomberman_bomb_left)++;
}

int can_drop_bomb(BOARD *board, BOMBERMAN *bomberman)
{
	int x, y;
	x = from_pixel_to_grid(board, bomberman->x, 1);
	y = from_pixel_to_grid(board, bomberman->y, 0);

	return (bomberman->bomb_left > 0 && board->grid[y][x].bomb == NULL);
}

void drop_bomb(BOARD *board, BOMBERMAN *bomberman)
{
	int x, y;

	x = from_pixel_to_grid(board, bomberman->x, 1);
	y = from_pixel_to_grid(board, bomberman->y, 0);

	board->grid[y][x].bomb = init_bomb(x, y, bomberman->bomb_power, &bomberman->bomb_left);
}

void free_bomb(BOARD *board, BOMB *bomb)
{
	board->grid[bomb->y][bomb->x].bomb = NULL;
	free(bomb);
}
