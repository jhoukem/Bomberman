#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

typedef struct BOMBERMAN BOMBERMAN;

void handle_key(SDL_Event *event, BOMBERMAN *bomberman, int dflag);
void handle_event(SDL_Event * event, int * play, BOMBERMAN *bomberman);

#endif // INPUT_H_INCLUDED
