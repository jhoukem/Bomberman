/*
 * assets.c
 *
 *  Created on: 18 nov. 2016
 *      Author: Jean-Hugo
 */

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "assets.h"

ASSETS* load_assets(SDL_Renderer *renderer, char *path_to_spritesheet)
{
	ASSETS *assets = malloc(sizeof(ASSETS));
	assets->wall.x = 0;
	assets->wall.y = 44;
	assets->ground.x = 120;
	assets->ground.y = 44;
	assets->wall.w = assets->wall.h = assets->ground.w = assets->ground.h = 15;
	assets->explosion.w = 16;
	assets->explosion.h = 16;
	assets->explosion.x = 150;
	assets->explosion.y = 25;

	assets->spritesheet = IMG_LoadTexture(renderer, path_to_spritesheet);
	if(assets->spritesheet == NULL){
		free_assets(assets);
		return NULL;
	}

	return assets;
}

void free_assets(ASSETS *assets)
{
	if(assets->spritesheet != NULL){
		SDL_DestroyTexture(assets->spritesheet);
	}
	assets->spritesheet = NULL;
	free(assets);
	assets = NULL;
}
