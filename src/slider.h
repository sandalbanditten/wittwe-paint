#ifndef SLIDER_H_
#define SLIDER_H_

#include <SDL2/SDL.h>
#include <assert.h>

#define SLIDER_THICCNESS 5.0f
#define SLIDER_GRIP_SIZE 15.0f

// Clamps a floating point value between two others
float clampf(float v, float lo, float hi);
// Lerps between two floats
float lerpf(float a, float b, float t);
// Lerps between two floats, but interpolated
float ilerpf(float a, float b, float v);
// Draws a slider, that modifies 'value'
void slider(SDL_Renderer *renderer, int id,
		float pos_x, float pos_y, float len,
		Uint8 *value, float min, float max,
		Uint8 size, Uint8 r, Uint8 g, Uint8 b);

#endif
