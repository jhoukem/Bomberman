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
#define ANIMATION_SPEED 250
#define SPEED 0.06
#define DEBUG 1
#define SPRITE_SHIFT 5
#define GROUND 0
#define WALL 1

SDL_Rect pos, coll;

void update_bomberman(BOARD *board, BOMBERMAN *bomberman, int l_size, int c_size)
{
	update_position(board, bomberman);
	update_bomberman_animation(bomberman);
}

void render_bomberman(SDL_Renderer *renderer, BOMBERMAN *bomberman, SDL_Texture *spritesheet, SDL_Rect *draw_pos)
{

	//Render sprite
	draw_pos->w = 24;
	draw_pos->h = 24;
	draw_pos->x = ((int)bomberman->x) - (draw_pos->w/2);
	draw_pos->y = ((int)bomberman->y) - (draw_pos->h/2 + SPRITE_SHIFT);
	SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);

	// Render the bomberman on the other side of the screen if he is outboud.
	if(draw_pos->x + (bomberman->sprite.w) > WIDTH){
		draw_pos->x =  - bomberman->sprite.w + (draw_pos->x + bomberman->sprite.w)% WIDTH;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}
	else if(draw_pos->x < 0){
		draw_pos->x =  WIDTH + draw_pos->x;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}
	if(draw_pos->y + (bomberman->sprite.h) > HEIGHT){
		draw_pos->y =  - bomberman->sprite.h + (draw_pos->y + bomberman->sprite.h)% HEIGHT;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}
	else if(draw_pos->y < 0){
		draw_pos->y =  HEIGHT + draw_pos->y;
		SDL_RenderCopy(renderer, spritesheet, &bomberman->sprite, draw_pos);
	}

	if(DEBUG){
		// Render pos
		pos.h = pos.w = 4;
		pos.x = bomberman->x - pos.w/2;
		pos.y = bomberman->y - pos.h/2;
		SDL_SetRenderDrawColor(renderer, 255, 255, 150, 1);
		SDL_RenderFillRect(renderer, &pos);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
		SDL_RenderDrawRect(renderer, &bomberman->hitbox);

	}

}


int hitbox_collide(BOARD *board, BOMBERMAN *bomberman, float next_y, float next_x,
		int next_y_in_tab, int next_x_in_tab)
{
	int next_row, next_col;

	// We check the 3 closest cells in the direction of the bomberman.

	// Update the hithox to the next position.
	bomberman->hitbox.y = (int) (((int)next_y) - bomberman->hitbox.h/2);
	bomberman->hitbox.x = (int) (((int)next_x) - bomberman->hitbox.w/2);

	//printf("next_y=%f\n", next_y);
	fflush(stdout);

	switch(bomberman->direction){
	case 0: // Down
		next_row = get_next_val(next_y_in_tab + 1, board->l_size);
		coll.y = next_row * (HEIGHT/board->l_size);
		coll.x = next_x_in_tab * (WIDTH/board->c_size);

		// Check map wrapping
		if(bomberman->hitbox.y + bomberman->hitbox.h >= HEIGHT){
			bomberman->hitbox.y = 0;
		}

		if(board->grid[next_row][next_x_in_tab].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_col = get_next_val(next_x_in_tab - 1, board->c_size);
		coll.x = next_col * (WIDTH/board->c_size);
		//printf("next_row=%d next_col=%d\n", next_row, next_col);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_col = get_next_val(next_x_in_tab + 1, board->c_size);
		coll.x = next_col * (WIDTH/board->c_size);
		//printf("next_row=%d next_col=%d\n", next_row, next_col);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		break;
	case 1: // Left
		next_col = get_next_val(next_x_in_tab - 1, board->c_size);
		coll.x = next_col * (WIDTH/board->c_size);
		coll.y = next_y_in_tab * (HEIGHT/board->l_size);

		// Check map wrapping
		if(bomberman->hitbox.x <= 0){
			bomberman->hitbox.x = WIDTH - bomberman->hitbox.w;
		}

		if(board->grid[next_y_in_tab][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_row = get_next_val(next_y_in_tab + 1, board->l_size);
		coll.y =  next_row * (HEIGHT/board->l_size);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_row = get_next_val(next_y_in_tab - 1, board->l_size);
		coll.y =  next_row * (HEIGHT/board->l_size);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		break;
	case 2: // Right
		next_col = get_next_val(next_x_in_tab + 1, board->c_size);
		coll.x = next_col * (WIDTH/board->c_size);
		coll.y = next_y_in_tab * (HEIGHT/board->l_size);

		// Check map wrapping
		if(bomberman->hitbox.x + bomberman->hitbox.w >= WIDTH){
			bomberman->hitbox.x = 0;
		}

		if(board->grid[next_y_in_tab][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_row = get_next_val(next_y_in_tab + 1, board->l_size);
		coll.y = next_row * (HEIGHT/board->l_size);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_row = get_next_val(next_y_in_tab - 1, board->l_size);
		coll.y = next_row * (HEIGHT/board->l_size);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		break;
	case 3: // Up
		next_row = get_next_val(next_y_in_tab - 1, board->l_size);
		coll.y = next_row * (HEIGHT/board->l_size);
		coll.x = next_x_in_tab * (WIDTH/board->c_size);

		// Check map wrapping
		if(bomberman->hitbox.y <= 0){
			bomberman->hitbox.y = HEIGHT - bomberman->hitbox.h;
		}

		if(board->grid[next_row][next_x_in_tab].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_col = get_next_val(next_x_in_tab - 1, board->c_size);
		coll.x = next_col * (WIDTH/board->c_size);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		next_col = get_next_val(next_x_in_tab + 1, board->c_size);
		coll.x = next_col * (WIDTH/board->c_size);
		if(board->grid[next_row][next_col].type != GROUND &&
				SDL_HasIntersection(&bomberman->hitbox, &coll)){
			return 1;
		}
		break;
	}
	return 0;
}




int can_go_over(BOARD *board, BOMBERMAN *bomberman, float next_y, float next_x,
		int next_y_in_tab, int next_x_in_tab)
{
	int x, y;
	x = from_pixel_to_grid_coord(board, bomberman->x, 1);
	y = from_pixel_to_grid_coord(board, bomberman->y, 0);

	coll.w = (WIDTH/board->c_size);
	coll.h = (HEIGHT/board->c_size);

	if(hitbox_collide(board, bomberman, next_y, next_x, next_y_in_tab, next_x_in_tab)){
		return 0;
	}

	// If the player is already on a bomb
	if(board->grid[y][x].bomb != NULL){
		// He can only move on this bomb or to a free cell.
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

	next_x = bomberman->x;
	next_y = bomberman->y;

	if(is_moving(bomberman)){
		switch(bomberman->direction){
		case 0:
			next_y = get_next_val(bomberman->y + bomberman->speed, HEIGHT);
			next_y_in_tab = from_pixel_to_grid_coord(board, next_y, 0);
			next_x_in_tab = from_pixel_to_grid_coord(board, bomberman->x, 1);

			if(can_go_over(board, bomberman, next_y, next_x, next_y_in_tab, next_x_in_tab)){
				board->grid[from_pixel_to_grid_coord(board, bomberman->y, 0)][next_x_in_tab].bomberman = NULL;
				bomberman->y = next_y;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		case 1:
			next_x = get_next_val(bomberman->x - bomberman->speed, WIDTH);
			next_x_in_tab = from_pixel_to_grid_coord(board, next_x, 1);
			next_y_in_tab = from_pixel_to_grid_coord(board, bomberman->y, 0);

			if(can_go_over(board, bomberman, next_y, next_x, next_y_in_tab, next_x_in_tab)){
				board->grid[next_y_in_tab][from_pixel_to_grid_coord(board, bomberman->x, 1)].bomberman = NULL;
				bomberman->x = next_x;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		case 2:
			next_x = get_next_val(bomberman->x + bomberman->speed, WIDTH);
			next_x_in_tab = from_pixel_to_grid_coord(board, next_x, 1);
			next_y_in_tab = from_pixel_to_grid_coord(board, bomberman->y, 0);

			if(can_go_over(board, bomberman, next_y, next_x, next_y_in_tab, next_x_in_tab)){
				board->grid[next_y_in_tab][from_pixel_to_grid_coord(board, bomberman->x, 1)].bomberman = NULL;
				bomberman->x = next_x;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		case 3:
			next_y = get_next_val(bomberman->y - bomberman->speed, HEIGHT);
			next_y_in_tab = from_pixel_to_grid_coord(board, next_y, 0);
			next_x_in_tab = from_pixel_to_grid_coord(board, bomberman->x, 1);

			if(can_go_over(board, bomberman, next_y, next_x, next_y_in_tab, next_x_in_tab)){
				board->grid[from_pixel_to_grid_coord(board, bomberman->y, 0)][next_x_in_tab].bomberman = NULL;
				bomberman->y = next_y;
				board->grid[next_y_in_tab][next_x_in_tab].bomberman = bomberman;
			}
			break;
		}
		bomberman->hitbox.x = bomberman->x - bomberman->hitbox.w/2;
		bomberman->hitbox.y = bomberman->y - bomberman->hitbox.h/2;
	}
}

int from_pixel_to_grid_coord(BOARD *board, float pos, int fwidth)
{
	int ret;
	if(fwidth){
		ret = (int)(pos /(WIDTH/board->c_size));
		return ret;// < board->c_size ? ret :  board->c_size - 1;
	}
	else {
		ret = (int)(pos /(HEIGHT/board->l_size));
		return ret;// <  board->l_size ? ret :  board->l_size - 1;
	}
}

float get_next_val(float val, float size)
{
	if(val >= size){
		return 0;
	}
	else if(val < 0) {
		return size - 1;
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
		bomberman->sprite.x = (sprite * bomberman->sprite.w) + (bomberman->direction * (FRAME_PER_ANIMATION * bomberman->sprite.w));
	} else {
		bomberman->sprite.x = bomberman->direction * (FRAME_PER_ANIMATION * bomberman->sprite.w);
	}
}

BOMBERMAN* alloc_bomberman(BOARD *board)
{
	BOMBERMAN *bomberman;
	bomberman = malloc(sizeof(BOMBERMAN));
	bomberman->x = board->c_size/2 * (WIDTH/board->c_size);
	bomberman->y = board->l_size/2 * (HEIGHT/board->l_size);
	bomberman->sprite.w = 16;
	bomberman->sprite.h = 24;
	bomberman->sprite.x = 0;
	bomberman->sprite.y = 0;
	bomberman->hitbox.w = 14;
	bomberman->hitbox.h = 10;
	bomberman->hitbox.x = bomberman->x - bomberman->hitbox.w/2;
	bomberman->hitbox.y = bomberman->y - bomberman->hitbox.h/2;

	bomberman->direction = 0;
	bomberman->bomb_left = 3;
	bomberman->bomb_power = 7;
	bomberman->move_down = bomberman->move_left = bomberman->move_right = bomberman->move_up = false;
	bomberman->speed = SPEED;

	return bomberman;
}

void free_bomberman(BOMBERMAN *bomberman)
{
	free(bomberman);
	bomberman = NULL;
}
