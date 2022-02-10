/* Slider related functions */

#include "slider.h"

static int activeId = -1;

// Helping functions
float clampf(float v, float lo, float hi)
{
	if (v < lo) v = lo;
	if (v > hi) v = hi;
	return v;
}

float lerpf(float a, float b, float t)
{
	return (b - a) * t + a;
}

float ilerpf(float a, float b, float v)
{
	return (v - a) / (b - a);
}

// The slider itself
void slider(SDL_Renderer *renderer, int id,
		float pos_x, float pos_y, float len,
		Uint8 *value, float min, float max,
		Uint8 size, Uint8 r, Uint8 g, Uint8 b)
{
	// TODO: display the current value of the slider

	// Slider Body
	{
		// If the slider is the size slider, make it a greyscale function of it's value
		// if it is an RGB slider, make it it's value in color
		SDL_Rect rect = {
			.x = pos_x,
			.y = pos_y - SLIDER_THICCNESS * 0.5f,
			.w = len,
			.h = SLIDER_THICCNESS,
		};
		// Outline and then color
		SDL_SetRenderDrawColor(renderer, r * (id == 2), g * (id == 3), b * (id == 4), 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect);
	}

	// Grip
	{
		assert(min <= max);
		float grip_value = ilerpf(min, max, *value) * len;

		// TODO: the grip should go outside of the slider body

		SDL_Rect rect = {
			.x = pos_x - SLIDER_GRIP_SIZE + grip_value,
			.y = pos_y - SLIDER_GRIP_SIZE,
			.w = SLIDER_GRIP_SIZE * 2.0f,
			.h = SLIDER_GRIP_SIZE * 2.0f,
		};
		// Outline and then color
		SDL_SetRenderDrawColor(renderer, 255 * (id == 2), 255 * (id == 3), 255 * (id == 4), 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect);

		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);

		// TODO: the grip should maintain the initial offset between its position and mouse_x

		if (activeId < 0) {
			SDL_Point cursor = {x, y};
			if (SDL_PointInRect(&cursor, &rect) && (buttons & SDL_BUTTON_LMASK) != 0) {
				activeId = id;
			}
		} else {
			if (activeId == id) {
				if ((buttons & SDL_BUTTON_LMASK) == 0) {
					activeId = -1;
				} else {
					float grip_min = pos_x - SLIDER_GRIP_SIZE;
					float grip_max = grip_min + len;
					float xf = clampf(x - SLIDER_GRIP_SIZE, grip_min, grip_max);
					xf = ilerpf(grip_min, grip_max, xf);
					xf = lerpf(min, max, xf);
					*value = xf;
				}
			}
		}
	}
}
