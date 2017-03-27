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
	assets->wall.y = 43;
	assets->wall_breakable.x = 17;
	assets->wall_breakable.y = 43;
	assets->ground.x = 119;
	assets->ground.y = 43;
	assets->bonus_bomb.x = 0;
	assets->bonus_bomb.y = 59;
	assets->bonus_power.x = 68;
	assets->bonus_power.y = 59;
	assets->bonus_speed.x = 17;
	assets->bonus_speed.y = 59;
	assets->malus.x = 85;
	assets->malus.y = 59;

	assets->wall.w = assets->wall.h = assets->ground.w = assets->ground.h =
			assets->wall_breakable.w = assets->wall_breakable.h = assets->bonus_bomb.w =
					assets->bonus_bomb.h = assets->bonus_power.w = assets->bonus_power.h =
							assets->bonus_speed.w = assets->bonus_speed.h =	assets->malus.w =
									assets->malus.h = 16;
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
	SDL_DestroyTexture(assets->spritesheet);
	assets->spritesheet = NULL;
	free(assets);
	assets = NULL;
}
