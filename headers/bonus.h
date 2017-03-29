#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef BONUS_H_INCLUDED
#define BONUS_H_INCLUDED

typedef struct BOMBERMAN BOMBERMAN;
typedef struct BOARD BOARD;

enum BONUS_TYPE{
	MOREPOWER, MORESPEED, MOREBOMB, MAXPOWER, MALUS
};
typedef enum BONUS_TYPE BONUS_TYPE;



struct BONUS{
	BONUS_TYPE type;
	SDL_Rect sprite;
};
typedef struct BONUS BONUS;


void apply_bonus_on_bomberman(BOARD *board, int y, int x, BOMBERMAN *bomberman);
void spawn_bonus(BOARD *board, ASSETS *assets, int y, int x);

#endif
