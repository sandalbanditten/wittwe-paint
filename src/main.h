#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>

#define NAME "Wittwe Paint"

#define WIDTH 1280
#define HEIGHT 720

#define SIZES 5
#define COLORS 4

#define SLIDER_THICCNESS 5.0f
#define SLIDER_GRIP_SIZE 15.0f

/* Variables */
static int active_id = -1;

/* Structs */
struct {
	Uint8 size;
	Uint8 r;
	Uint8 g;
	Uint8 b;
} Brush;

/* Functions */

void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
Uint8 RectCheckBounds(int x, int y, struct SDL_Rect Rect);
static inline float clampf(float v, float lo, float hi);
static inline float lerpf(float a, float b, float t);
static inline float ilerpf(float a, float b, float v);
static void slider(SDL_Renderer *renderer, int id,
		float pos_x, float pos_y, float len,
		Uint8 *value, float min, float max);

#endif
