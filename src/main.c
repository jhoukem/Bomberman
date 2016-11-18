
#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL_Image.h>
#include "input.h"
#include "board.h"
#include "bomberman.h"

#define SIZE 20
#define WIDTH 480
#define HEIGHT 480

int init_rsc(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **spritesheet)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Error cannot initialize the SDL2: %s\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow(
                  "Bomberman",
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  WIDTH,
                  HEIGHT,
                  SDL_WINDOW_OPENGL);
    if (*window == NULL)
    {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 0;
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(*renderer == NULL)
    {
        fprintf(stderr, "Error cannot create SDL_renderer: %s\n", SDL_GetError());
        return 0;
    }
    // Set size of renderer to the same as window
    SDL_RenderSetLogicalSize(*renderer, WIDTH, HEIGHT);
    *spritesheet = IMG_LoadTexture(*renderer, "rsc/spritesheet.png");
    if(*spritesheet == NULL)
    {
        fprintf(stderr, "Failed to load texture: %s", SDL_GetError());
    }

    return 1;
}



int run_game(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *spritesheet, int size)
{
	int play;
	play = 1;

	// Initialisation.
    SDL_Event event;
    BOARD *board;
    BOMBERMAN *bomberman;

    board = alloc_board(size, size);
    bomberman = alloc_bomberman(board);

    // The rec representing the wall and the ground on the spritesheet
    SDL_Rect wall, ground;
    wall.x = 0;
    wall.y = 44;
    ground.x = 120;
    ground.y = 44;
    wall.w = wall.h = ground.w = ground.h = 15;

    while(play)
    {
        handle_event(&event, &play, bomberman);

        // Clear the screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        display_board(board, renderer, spritesheet, &wall, &ground, bomberman);
    }
    free_board(board, size);
    free_bomberman(bomberman);
    return 0 ;
}

void free_rsc(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *spritesheet)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(spritesheet);
    SDL_Quit();
}



int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* spritesheet;
    if(!init_rsc(&window, &renderer, &spritesheet))
    {
        return -1;
    }
    run_game(window, renderer, spritesheet, SIZE);
    free_rsc(window, renderer, spritesheet);
    return 0;
}
