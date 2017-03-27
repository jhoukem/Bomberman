
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
#include "audio_param.h"

#define SIZE 20
#define WIDTH 480
#define HEIGHT 480
#define FPS 60

int init_rsc(SDL_Window **window, GRAPHIC_PARAM **g_param, AUDIO_PARAM ** a_param)
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
	(*g_param)->game_over_surface = NULL;
	(*g_param)->game_paused_surface = NULL;
	(*g_param)->press_escape_surface = NULL;
	(*g_param)->game_over_texture = NULL;
	(*g_param)->game_paused_texture = NULL;
	(*g_param)->press_escape_texture = NULL;
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
	(*g_param)->game_over_surface = TTF_RenderText_Blended((*g_param)->font,
			"game over", color);
	(*g_param)->press_escape_surface = TTF_RenderText_Blended((*g_param)->font,
				"press escape", color);
	(*g_param)->game_paused_surface = TTF_RenderText_Blended((*g_param)->font,
			"pause", color);
	(*g_param)->game_over_texture = SDL_CreateTextureFromSurface((*g_param)->renderer, (*g_param)->game_over_surface);
	(*g_param)->game_paused_texture = SDL_CreateTextureFromSurface((*g_param)->renderer, (*g_param)->game_paused_surface);
	(*g_param)->press_escape_texture = SDL_CreateTextureFromSurface((*g_param)->renderer, (*g_param)->press_escape_surface);

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096 ) == -1 ){
		fprintf(stderr, "Error failed to open audio, Mix_Error: %s\n", Mix_GetError());
		return -1;
	}

	*a_param = malloc(sizeof(**a_param));
	(*a_param)->ambiance = Mix_LoadMUS("rsc/battle2.mp3");
	(*a_param)->explosion = Mix_LoadWAV("rsc/explosion.wav");
	(*a_param)->power_up = Mix_LoadWAV("rsc/power_up.wav");
	Mix_AllocateChannels(2);
	Mix_VolumeChunk((*a_param)->explosion, MIX_MAX_VOLUME/4);
	Mix_VolumeChunk((*a_param)->power_up, MIX_MAX_VOLUME/4);
	Mix_VolumeMusic(MIX_MAX_VOLUME/4);
	return 1;
}

void reset_game(BOARD *board, BOMBERMAN *bomberman, ASSETS *assets)
{
	int alea = 0;
	reset_board(board);
	reset_bomberman(bomberman, board);
	// Change the assets.
	alea = rand()%4;
	assets->ground.x = 68 + 1 * alea + alea * assets->ground.w;
	alea = rand()%3;
	assets->wall_breakable.x = 17 + 1 * (alea) + alea * assets->wall_breakable.w;
}

int run_game(SDL_Window *window, GRAPHIC_PARAM *g_param, AUDIO_PARAM *a_param, int size)
{
	BOARD *board;
	BOMBERMAN *bomberman;
	SDL_Event event;
	SDL_bool play, pause, reset, is_game_over;
	Uint32 current_time, previous_time, mspf;

	// Init variables
	current_time = 0;
	previous_time = 0;
	play = SDL_TRUE;
	pause = reset = is_game_over = SDL_FALSE;
	mspf = 1000/FPS;

	// Alloc game structures.
	board = alloc_board(size, size);
	bomberman = alloc_bomberman(board);

	Mix_PlayMusic(a_param->ambiance, -1);

	while(play)
	{

		if(reset){
			reset_game(board, bomberman, g_param->assets);
			reset = SDL_FALSE;
			is_game_over = SDL_FALSE;
		}

		current_time = SDL_GetTicks();
		if (current_time - previous_time > mspf){
			previous_time = current_time;

			handle_event(&event, &play, &pause, &reset, is_game_over, board, bomberman);

			// Clear the screen.
			SDL_SetRenderDrawColor(g_param->renderer, 0, 0, 0, 1);
			SDL_RenderClear(g_param->renderer);

			display_board(g_param, board, bomberman);
			if(!is_game_over){
				if(!pause){
					is_game_over = update_board(g_param, a_param, board, bomberman);
				}
			}
			display_status(g_param, pause, is_game_over);

			SDL_RenderPresent(g_param->renderer);

		} else {
			SDL_Delay(mspf - (current_time - previous_time));
		}
	}

	free_board(board);
	free_bomberman(bomberman);
	return 0 ;
}

void free_audio(AUDIO_PARAM *a_param)
{
	Mix_FreeMusic(a_param->ambiance);
	Mix_FreeChunk(a_param->explosion);
	Mix_FreeChunk(a_param->power_up);
	free(a_param);
}

void free_graphics(GRAPHIC_PARAM *g_param)
{
	free_assets(g_param->assets);
	TTF_CloseFont(g_param->font);
	SDL_FreeSurface(g_param->game_over_surface);
	SDL_FreeSurface(g_param->game_paused_surface);
	SDL_FreeSurface(g_param->press_escape_surface);
	SDL_DestroyRenderer(g_param->renderer);
	free(g_param->game_over_texture);
	free(g_param->game_paused_texture);
	free(g_param->press_escape_texture);
	free(g_param->draw_pos);
	free(g_param);
}

void free_rsc(SDL_Window *window, GRAPHIC_PARAM *g_param, AUDIO_PARAM *a_param)
{
	free_graphics(g_param);
	free_audio(a_param);
	TTF_Quit();
	Mix_CloseAudio();
	SDL_DestroyWindow(window);
	SDL_Quit();
}



int main(int argc, char *argv[])
{

	SDL_Window *window;
	GRAPHIC_PARAM *g_param;
	AUDIO_PARAM *a_param;

	if(!init_rsc(&window, &g_param, &a_param)){
		exit(EXIT_FAILURE);
	}

	run_game(window, g_param, a_param, SIZE);
	free_rsc(window, g_param, a_param);

	return 0;
}
