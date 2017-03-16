/*
 * ai.h
 *
 *  Created on: 19 déc. 2016
 *      Author: Jean-Hugo
 */

#ifndef AI_H_
#define AI_H_

typedef struct BOARD BOARD;
typedef struct BOMBERMAN BOMBERMAN;


void update_ai_bomberman(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction);
SDL_bool is_dangerous_area(BOARD *board, int y, int x);
void set_new_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction, int y, int x);
void set_new_defense_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction,
		int y, int x);
void set_new_offense_goal(BOARD *board, BOMBERMAN *bomberman, int y, int x);
int get_direction_to_closest_safe_cell(BOARD *board, int **grid_iteration, int **grid_direction, int y, int x);
void try_to_drop_bomb(BOARD *board, BOMBERMAN *bomberman);
SDL_bool is_around_safe(BOARD *board, int y, int x);
#endif /* AI_H_ */
