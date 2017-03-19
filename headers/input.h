#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#endif

#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

typedef struct BOMBERMAN BOMBERMAN;
typedef struct BOARD BOARD;

void handle_key(SDL_Event *event, BOARD *board, BOMBERMAN *bomberman, int dflag);
void handle_event(SDL_Event * event, int * play, BOARD *board, BOMBERMAN *bomberman);

#endif // INPUT_H_INCLUDED
