
#include <assets.h>
#include <board.h>
#include <bomberman.h>
#include <input.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "graphic_param.h"

#define SIZE 20
#define WIDTH 480
#define HEIGHT 480
#define FPS 60

int init_rsc(SDL_Window **window, GRAPHIC_PARAM **g_param)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		fprintf(stderr, "Error failed to initialize the SDL2: %s\n", SDL_GetError());
		return -1;
	}

	*window = SDL_CreateWindow(
			"Bomberman",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WIDTH,
			HEIGHT,
			SDL_WINDOW_OPENGL);

	if(*window == NULL){
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		return -1;
	}


	*g_param = malloc(sizeof(**g_param));
	(*g_param)->assets = NULL;
	(*g_param)->font = NULL;
	(*g_param)->renderer = NULL;
	(*g_param)->status_surface = NULL;
	(*g_param)->status_texture = NULL;
	(*g_param)->draw_pos = malloc(sizeof(*(*g_param)->draw_pos));
	(*g_param)->renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

	if((*g_param)->renderer == NULL){
		fprintf(stderr, "Error cannot create SDL_renderer: %s\n", SDL_GetError());
		return -1;
	}
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize((*g_param)->renderer, WIDTH, HEIGHT);
	(*g_param)->assets = load_assets((*g_param)->renderer, "rsc/spritesheet.png");
	if((*g_param)->assets == NULL){
		fprintf(stderr, "Failed to load assets: %s", SDL_GetError());
		return -1;
	}

	if(TTF_Init() == -1){
		fprintf(stderr, "Error failed to initialize SDL_TTF : %s\n", TTF_GetError());
		return -1;
	}
	(*g_param)->font = TTF_OpenFont("rsc/bm.ttf", 32);
	if((*g_param)->font == NULL){
		fprintf(stderr, "Error failed to open font, TTF_Error: %s\n", TTF_GetError());
		return -1;
	}
	SDL_Color color = {255, 140, 0};
	(*g_param)->status_surface = TTF_RenderText_Blended((*g_param)->font, "Game Over", color);
	(*g_param)->status_texture = SDL_CreateTextureFromSurface((*g_param)->renderer, (*g_param)->status_surface);

	// Change param 1 to 44100,  ??
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096 ) == -1 ){
		fprintf(stderr, "Error failed to open audio, Mix_Error: %s\n", Mix_GetError());
		return -1;
	}

	return 1;
}



int run_game(SDL_Window *window, GRAPHIC_PARAM *g_param, int size)
{
	int play, status;
	BOARD *board;
	BOMBERMAN *bomberman;
	SDL_Event event;
	Uint32 current_time, previous_time, mspf;

	// Init variables
	play = 1;
	status = 0;
	current_time = 0;
	previous_time = 0;
	mspf = 1000/FPS;

	// Alloc game structures.
	board = alloc_board(size, size);
	bomberman = alloc_bomberman(board);

	while(play)
	{
		current_time = SDL_GetTicks();
		if (current_time - previous_time > mspf){
			previous_time = current_time;

			handle_event(&event, &play, board, bomberman);

			// Clear the screen.
			SDL_SetRenderDrawColor(g_param->renderer, 0, 0, 0, 1);
			SDL_RenderClear(g_param->renderer);

			display_board(g_param, board, bomberman);
			status = update_board(g_param, board, bomberman);
			display_status(g_param, status);

			SDL_RenderPresent(g_param->renderer);

		} else {
			SDL_Delay(mspf - (current_time - previous_time));
		}
	}

	free_board(board);
	free_bomberman(bomberman);
	return 0 ;
}


void free_graphics(GRAPHIC_PARAM *g_param)
{
	free_assets(g_param->assets);
	TTF_CloseFont(g_param->font);
	SDL_FreeSurface(g_param->status_surface);
	SDL_DestroyRenderer(g_param->renderer);
	free(g_param->status_texture);
	free(g_param->draw_pos);
	free(g_param);
}

void free_rsc(SDL_Window *window, GRAPHIC_PARAM *g_param)
{
	free_graphics(g_param);
	SDL_DestroyWindow(window);
	TTF_Quit();
	Mix_CloseAudio();
	SDL_Quit();
}



int main(int argc, char *argv[])
{
	SDL_Window *window;
	GRAPHIC_PARAM *g_param;

	if(!init_rsc(&window, &g_param)){
		exit(EXIT_FAILURE);
	}
	run_game(window, g_param, SIZE);
	free_rsc(window, g_param);
	return 0;
}
