/*
 * graphic_param.h
 *
 *  Created on: 23 mars 2017
 *      Author: Jean-Hugo
 */

#ifndef GRAPHIC_PARAM_H_
#define GRAPHIC_PARAM_H_

struct GRAPHIC_PARAM{

	SDL_Renderer *renderer;
	ASSETS *assets;
	SDL_Rect *draw_pos;
	SDL_Surface *status_surface;
	SDL_Texture *status_texture;
	TTF_Font *font;

};

typedef struct GRAPHIC_PARAM GRAPHIC_PARAM;

#endif /* GRAPHIC_PARAM_H_ */
