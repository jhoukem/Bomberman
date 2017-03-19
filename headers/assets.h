/*
 * scenery.h
 *
 *  Created on: 18 nov. 2016
 *      Author: Jean-Hugo
 */

#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#endif

#ifndef ASSETS_H_
#define ASSETS_H_

typedef struct ASSETS ASSETS;

#define ANIMATION_TYPE 3
#define ANIMATION_FRAMES 5

struct ASSETS{
	SDL_Rect wall, wall_breakable, ground, explosion, bonus_bomb, bonus_power, bonus_speed, malus;
	SDL_Texture *spritesheet; // The spritesheet with all the texture.
};

ASSETS* load_assets(SDL_Renderer *renderer, char *path_to_spritesheet);
void free_assets(ASSETS *assets);

#endif /* ASSETS_H_ */
