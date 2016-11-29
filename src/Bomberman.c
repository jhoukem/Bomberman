/*
 * bomberman.c
 *
 *  Created on: 17 nov. 2016
 *      Author: Jean-Hugo
 */
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "board.h"
#include "bomberman.h"
#include "bomb.h"


#define WIDTH 480
#define HEIGHT 480

#define FRAME_PER_ANIMATION 3
#define SPRITE_WIDTH 15
#define ANIMATION_WIDTH 49
#define ANIMATION_SPEED 250
#define SPEED 0.02
#define DEBUG 0

SDL_Rect pos;

void update_bomberman(BOARD *board, BOMBERMAN *bomberman, int l_size, int c_size)
{
	update_position(board, bomberman);
	update_bomberman_animation(bomberman);
}

void render_bomberman(SDL_Renderer *renderer, BOMBERMAN *bomberman, SDL_Texture *spritesheet, SDL_Rect *draw_pos)
{

	//Render srpite
	draw_pos->w = 24;
	draw_pos->h = 24;
	draw_pos->x = bomberman->x - (bomberman->sprite.w/2);
	draw_pos->y = bomberman->y - (bomberman->sprite.h/2);
	SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);

	// Render the bomberman on the other side of the screen if he is outboud.
	if(draw_pos->x + (bomberman->sprite.w) > WIDTH){
		//printf("Offbound: %d\n", draw_pos->x + (bomberman->sprite->w));
		//printf("Inbound: %d\n", (draw_pos->x + bomberman->sprite->w)% WIDTH);
		draw_pos->x =  - bomberman->sprite.w + (draw_pos->x + bomberman->sprite.w)% WIDTH;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}
	else if(draw_pos->x < 0){
		draw_pos->x =  WIDTH + draw_pos->x;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}
	else if(draw_pos->y + (bomberman->sprite.h) > HEIGHT){
		draw_pos->y =  - bomberman->sprite.h + (draw_pos->y + bomberman->sprite.h)% HEIGHT;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}
	else if(draw_pos->y < 0){
		draw_pos->y =  HEIGHT + draw_pos->y;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}


	// Render pos
	pos.h = pos.w = 1;
	pos.x = 2;
	pos.y = 61;
	draw_pos->w = 3;
	draw_pos->h = 3;

	draw_pos->x = bomberman->x;
	draw_pos->y = bomberman->y;
	SDL_RenderCopy(renderer, spritesheet, &pos, draw_pos);
	draw_pos->w = 24;
	draw_pos->h = 24;

}


int can_go_over(BOARD *board, BOMBERMAN *bomberman,  int next_y_in_tab, int next_x_in_tab)
{
	int x, y;
	x = from_pixel_to_grid(board, bomberman->x, 1);
	y = from_pixel_to_grid(board, bomberman->y, 0);

	// If the next position is not walkable.
	if(board->grid[next_y_in_tab][next_x_in_tab].type != 0)
		return 0;
	// If the player is already on a bomb
	if(board->grid[y][x].bomb != NULL){
		// He can only move on this bomb or a free cell.
		return (board->grid[next_y_in_tab][next_x_in_tab].bomb == board->grid[y][x].bomb) ||
				board->grid[next_y_in_tab][next_x_in_tab].bomb == NULL;
	} else {
		return board->grid[next_y_in_tab][next_x_in_tab].bomb == NULL;
	}
}

void update_position(BOARD *board, BOMBERMAN *bomberman)
{
	float next_y, next_x;
	int next_y_in_tab, next_x_in_tab;

	if(is_moving(bomberman)){
		switch(bomberman->direction){
		case 0:
			next_y = get_next_int(bomberman->y + bomberman->speed, HEIGHT);
			next_y_in_tab = from_pixel_to_grid(board, next_y, 0);
			next_x_in_tab = from_pixel_to_grid(board, bomberman->x, 1);
			if(DEBUG){
				//printf("ny=%d, nx=%d\n", next_y_in_tab, next_x_in_tab);
				printf("grid[%d][%d]=%d is bomb ? %p\n", next_y_in_tab, next_x_in_tab,
						board->grid[next_y_in_tab][next_x_in_tab].type,board->grid[next_y_in_tab][next_x_in_tab].bomb);
			}

			if(can_go_over(board, bomberman, next_y_in_tab, next_x_in_tab)){
				board->grid[from_pixel_to_grid(board, bomberman->y, 0)][next_x_in_tab].bomberman = NULL;
				bomberman->y = next_y;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		case 1:
			next_x = get_next_int(bomberman->x - bomberman->speed, WIDTH);
			next_x_in_tab = from_pixel_to_grid(board, next_x, 1);
			next_y_in_tab = from_pixel_to_grid(board, bomberman->y, 0);

			if(DEBUG){
				//printf("ny=%d, nx=%d\n", next_y_in_tab, next_x_in_tab);
				printf("grid[%d][%d]=%d\n", next_y_in_tab, next_x_in_tab,
						board->grid[next_y_in_tab][next_x_in_tab].type);
			}

			if(can_go_over(board, bomberman, next_y_in_tab, next_x_in_tab)){
				board->grid[next_y_in_tab][from_pixel_to_grid(board, bomberman->x, 1)].bomberman = NULL;
				bomberman->x = next_x;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		case 2:
			next_x = get_next_int(bomberman->x + bomberman->speed, WIDTH);
			next_x_in_tab = from_pixel_to_grid(board, next_x, 1);
			next_y_in_tab = from_pixel_to_grid(board, bomberman->y, 0);

			if(DEBUG){
				//printf("ny=%d, nx=%d\n", next_y_in_tab, next_x_in_tab);
				printf("grid[%d][%d]=%d\n", next_y_in_tab, next_x_in_tab,
						board->grid[next_y_in_tab][next_x_in_tab].type);
			}

			if(can_go_over(board, bomberman, next_y_in_tab, next_x_in_tab)){
				board->grid[next_y_in_tab][from_pixel_to_grid(board, bomberman->x, 1)].bomberman = NULL;
				bomberman->x = next_x;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		case 3:
			next_y = get_next_int(bomberman->y - bomberman->speed, HEIGHT);
			next_y_in_tab = from_pixel_to_grid(board, next_y, 0);
			next_x_in_tab = from_pixel_to_grid(board, bomberman->x, 1);

			if(DEBUG){
				//printf("ny=%d, nx=%d\n", next_y_in_tab, next_x_in_tab);
				printf("grid[%d][%d]=%d\n", next_y_in_tab, next_x_in_tab,
						board->grid[next_y_in_tab][next_x_in_tab].type);
			}

			if(can_go_over(board, bomberman, next_y_in_tab, next_x_in_tab)){
				board->grid[from_pixel_to_grid(board, bomberman->y, 0)][next_x_in_tab].bomberman = NULL;
				bomberman->y = next_y;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		}
	}
}

int from_pixel_to_grid(BOARD *board, float pos, int fwidth)
{
	int ret;
	if(fwidth){
		ret = (int)(pos /(WIDTH/board->c_size));
		return ret < board->c_size ? ret :  board->c_size - 1;
	}
	else {
		ret = (int)(pos /(HEIGHT/board->l_size));
		return ret <  board->l_size ? ret :  board->l_size - 1;
	}
}

float get_next_int(float val, int size)
{
	if(val > size){
		return 0;
	}
	else if(val < 0) {
		return size;
	}
	return val;
}

Bool is_moving(BOMBERMAN *bomberman)
{
	return (bomberman->move_down || bomberman->move_left || bomberman->move_right || bomberman->move_up);
}

void update_bomberman_animation(BOMBERMAN *bomberman)
{
	Uint32 sprite = (SDL_GetTicks() / ANIMATION_SPEED) % FRAME_PER_ANIMATION;
	if(is_moving(bomberman)){
		bomberman->sprite.x = (sprite * SPRITE_WIDTH) + (bomberman->direction * ANIMATION_WIDTH);
	} else {
		bomberman->sprite.x = bomberman->direction * ANIMATION_WIDTH;
	}
}

BOMBERMAN* alloc_bomberman(BOARD *board)
{
	BOMBERMAN *bomberman;
	bomberman = malloc(sizeof(BOMBERMAN));
	bomberman->sprite.w = 15;
	bomberman->sprite.h = 24;
	bomberman->sprite.x = 0;
	bomberman->sprite.y = 0;
	bomberman->x = board->c_size/2 * (WIDTH/board->c_size);
	bomberman->y = board->l_size/2 * (HEIGHT/board->l_size);
	bomberman->direction = 0;
	bomberman->bomb_left = 6;
	bomberman->bomb_power = 5;
	bomberman->move_down = bomberman->move_left = bomberman->move_right = bomberman->move_up = false;
	bomberman->speed = SPEED;

	return bomberman;
}

void free_bomberman(BOMBERMAN *bomberman)
{
	free(bomberman);
	bomberman = NULL;
}
