/*
 * graphic_param.h
 *
 *  Created on: 23 mars 2017
 *      Author: Jean-Hugo
 */
#ifndef SDL2
#define SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifndef GRAPHIC_PARAM_H_
#define GRAPHIC_PARAM_H_

struct GRAPHIC_PARAM{

	SDL_Renderer *renderer;
	ASSETS *assets;
	SDL_Rect *draw_pos;
	SDL_Surface *game_over_surface;
	SDL_Surface *game_paused_surface;
	SDL_Surface *press_escape_surface;
	SDL_Texture *game_over_texture;
	SDL_Texture *game_paused_texture;
	SDL_Texture *press_escape_texture;
	TTF_Font *font;

};
typedef struct GRAPHIC_PARAM GRAPHIC_PARAM;

#endif /* GRAPHIC_PARAM_H_ */
