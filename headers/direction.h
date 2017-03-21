/*
 * direction.h
 *
 *  Created on: 21 mars 2017
 *      Author: Jean-Hugo
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

enum DIRECTION{
	// Center only used for bomb explosions.
	DOWN=0, LEFT=1, RIGHT=2, UP=3, CENTER=4
};
typedef enum DIRECTION DIRECTION;

#endif /* DIRECTION_H_ */
