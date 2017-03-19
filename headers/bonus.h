#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#endif

#ifndef BONUS_H_INCLUDED
#define BONUS_H_INCLUDED

typedef struct BOMBERMAN BOMBERMAN;


enum BONUS_TYPE{
	MOREPOWER, MORESPEED, MOREBOMB, MALUS
};
typedef enum BONUS_TYPE BONUS_TYPE;



struct BONUS{
	BONUS_TYPE type;
	SDL_Rect sprite;
};
typedef struct BONUS BONUS;


void apply_bonus_on_bomberman(BONUS *bonus, BOMBERMAN *bomberman);

#endif
