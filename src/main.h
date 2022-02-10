#ifndef MAIN_H_
#define MAIN_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "slider.h"
#include "circle.h"
#include "image.h"

#define NAME "Wittwe Paint"

#define WIDTH 1920
#define HEIGHT 1080

struct {
	Uint8 size;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_Point loc;
} Brush;

#endif
