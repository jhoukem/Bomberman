#include <stdio.h>
#include "input.h"
#include "bomberman.h"

#define SIZE 20

void handle_keydown(SDL_Event *event, BOMBERMAN *bomberman)
{
    //printf("bomberman x=%d\n y=%d\n", bomberman->x, bomberman->y);
    switch(event->key.keysym.sym)
    {
    case SDLK_DOWN:
    	bomberman->y = get_real_int(bomberman->y + 1, SIZE);
        bomberman->direction = 0;
        break;
    case SDLK_LEFT:
    	bomberman->x = get_real_int(bomberman->x - 1, SIZE);
    	bomberman->direction = 1;
        break;
    case SDLK_RIGHT:
    	bomberman->x = get_real_int(bomberman->x + 1, SIZE);
    	bomberman->direction = 2;
        break;
    case SDLK_UP:
    	bomberman->y = get_real_int(bomberman->y - 1, SIZE);
    	bomberman->direction = 3;
        break;
    }

}

void handle_event(SDL_Event *event, int *play, BOMBERMAN *bomberman)
{

    if(SDL_PollEvent(event) != 0)
    {
        switch(event->type)
        {
        case SDL_QUIT:
            *play = 0;
            break;
        case SDL_KEYDOWN:
            handle_keydown(event, bomberman);
            break;
        }

    }
}

int get_real_int(int val, int size)
{
    if(val < 0)
    {
        return size - 1;
    }
    return val % size;
}
