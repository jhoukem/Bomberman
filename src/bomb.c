#include <stdlib.h>
#include <SDL2/SDL.h>
#include "bomb.h"
#include "bomberman.h"
#include "board.h"

BOMB *init_bomb(int x, int y, int power, int *bomberman_bomb_left)
{
	BOMB *bomb;
	bomb = malloc(sizeof(BOMB));
	bomb->x = x;
	bomb->y = y;
	bomb->has_explode = false;
	bomb->bomberman_bomb_left = bomberman_bomb_left;
	bomb->timer = 9000;
	bomb->power = power;
	bomb->sprite.x = 0;
	bomb->sprite.y = 30;
	bomb->sprite.w = 16;
	bomb->sprite.h = 17;
	(*bomb->bomberman_bomb_left)--;
	return bomb;
}
/*
void update_animation(BOMB *bomb)
{
	Uint32 sprite = (SDL_GetTicks() / ANIMATION_SPEED) % FRAME_PER_ANIMATION;
	if(!bomb->has_explode){
		bomb->sprite.x = (sprite * SPRITE_WIDTH);
	} else {
		bomberman->sprite.x = bomberman->direction * ANIMATION_WIDTH;
	}
}*/

void explode(BOARD *board, BOMB *bomb)
{
	int x, y;

	// Top
	for(y = bomb->y; y >= 0 && (-(y - bomb->y)) <= bomb->power; y--){
		board->grid[y][bomb->x].is_under_explosion = true;
	}

	// Bottom
	for(y = bomb->y; y < board->l_size && (y - bomb->y) <= bomb->power; y++){
		board->grid[y][bomb->x].is_under_explosion = true;
	}

	// Left
	for(x = bomb->x; x >= 0 && (-(x - bomb->x)) <= bomb->power; x--){
		board->grid[bomb->y][x].is_under_explosion = true;
	}

	// Right
	for(x = bomb->x; x < board->c_size && (x - bomb->x) <= bomb->power; x++){
		board->grid[bomb->y][x].is_under_explosion = true;
	}

	bomb->has_explode = true;
	bomb->timer = 800;
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
	int x, y;
	// Top
	for(y = bomb->y; y >= 0 && (-(y - bomb->y)) <= bomb->power; y--){
		board->grid[y][bomb->x].is_under_explosion = false;
	}

	// Bottom
	for(y = bomb->y; y < board->l_size && (y - bomb->y) <= bomb->power; y++){
		board->grid[y][bomb->x].is_under_explosion = false;
	}

	// Left
	for(x = bomb->x; x >= 0 && (-(x - bomb->x)) <= bomb->power; x--){
		board->grid[bomb->y][x].is_under_explosion = false;
	}

	// Right
	for(x = bomb->x; x < board->c_size && (x - bomb->x) <= bomb->power; x++){
		board->grid[bomb->y][x].is_under_explosion = false;
	}

	board->grid[bomb->y][bomb->x].bomb = NULL;
	free(bomb);
}
