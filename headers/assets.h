/*
 * scenery.h
 *
 *  Created on: 18 nov. 2016
 *      Author: Jean-Hugo
 */

#ifndef ASSETS_H_
#define ASSETS_H_

typedef struct ASSETS ASSETS;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Rect SDL_Rect;
typedef struct SDL_Texture SDL_Texture;

#define ANIMATION_TYPE 3
#define ANIMATION_FRAMES 5

struct ASSETS{
	SDL_Rect wall, wall_breakable, ground, explosion;

	//150,25
	//SDL_Rect explosion[ANIMATION_TYPE][ANIMATION_FRAMES];
	SDL_Texture *spritesheet; // The spritesheet with all the texture.
};

ASSETS* load_assets(SDL_Renderer *renderer, char *path_to_spritesheet);
void free_assets(ASSETS *assets);

#endif /* ASSETS_H_ */
