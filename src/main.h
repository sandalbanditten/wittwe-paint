#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define NAME "Wittwe Paint"

#define WIDTH 1280
#define HEIGHT 720

#define SIZES 5
#define COLORS 4

/* Variables */
Uint8 sizes[SIZES];

/* Structs */

struct {
	Uint8 size;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} Brush;

struct Circle {
	int x;
	int y;
	unsigned int diameter;
} sizeCircles[5];

struct SDL_Rect colorRects[4];

/* Functions */

void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
Uint8 RectCheckBounds(int x, int y, struct SDL_Rect Rect);
Uint8 CircleCheckBounds(int x, int y, Uint8 d);

#endif
