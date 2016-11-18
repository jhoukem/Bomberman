#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL2/SDL.h>
typedef struct BOMBERMAN BOMBERMAN;

int get_real_int(int val, int size);
void handle_keydown(SDL_Event *event, BOMBERMAN *bomberman);
void handle_event(SDL_Event * event, int * play, BOMBERMAN *bomberman);

#endif // INPUT_H_INCLUDED
