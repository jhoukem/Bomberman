/*
 * ai.h
 *
 *  Created on: 19 déc. 2016
 *      Author: Jean-Hugo
 */
#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef AI_H_
#define AI_H_

typedef struct BOARD BOARD;
typedef struct BOMBERMAN BOMBERMAN;
typedef enum DIRECTION DIRECTION;


void update_ai_bomberman(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, DIRECTION **grid_direction);
SDL_bool try_to_drop_bomb(BOARD *board, BOMBERMAN *bomberman);
void get_to_goal(BOMBERMAN *bomberman);
SDL_bool has_goal(BOMBERMAN * bomberman);
SDL_bool goal_no_more_available(BOARD *board, BOMBERMAN *bomberman);
void set_new_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, DIRECTION **grid_direction, int y, int x);
void set_new_defense_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, DIRECTION **grid_direction,
		int y, int x);
void set_new_offense_goal(BOARD *board, BOMBERMAN *bomberman, int **grid_iteration, int **grid_direction, int y, int x);
DIRECTION get_direction_to_closest_safe_cell(BOARD *board, int **grid_iteration, DIRECTION **grid_direction, int y, int x);
void mark_around_position(BOARD *board, int **grid_iteration, DIRECTION **grid_direction, int y, int x, int iteration);
SDL_bool safe_cell_around_exist_after_dropping_bomb(BOARD *board, int y, int x);
SDL_bool safe_cell_around_exist(BOARD *board, int y, int x, DIRECTION dir_to_exclude);
SDL_bool is_dangerous_area(BOARD *board, int y, int x);
SDL_bool is_safe_area_around(BOARD *board, int y, int x, int axis_to_include);
SDL_bool need_wrapping(BOARD *board, BOMBERMAN * bomberman);
SDL_bool reached_his_goal(BOMBERMAN *bomberman);

#endif /* AI_H_ */
