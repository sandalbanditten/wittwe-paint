/*
 * Dedicated to honey <3
 */

#include "main.h"
#include "circle.h"

int main(int argc, char **argv) {
	printf("Hello, World!\n");

	// Initialising SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		SDL_Log("Error: Unable to initialize SDL: %s", SDL_GetError());
		exit(1);
	}

	// Creating the window
	SDL_Window* window = SDL_CreateWindow(
			NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH, HEIGHT, 0);

	if (!window) {
		SDL_Log("Error: Unable to create a window: %s", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	// Creating the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(
			window, -1, SDL_RENDERER_ACCELERATED
			);

	if (!renderer) {
		SDL_DestroyWindow(window);
		SDL_Log("Error: Unable to create a renderer: %s", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	// The grey rectangle for the UI
	SDL_Rect UIrect = {
		.w = WIDTH,
		.h = 100,
		.x = 0,
		.y = HEIGHT - 100
	};

	Brush.size = 25;
	Brush.r = 0;
	Brush.g = 0;
	Brush.b = 0;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Main loop
	int quit = 0;
	while (!quit) {
		// Check for event
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = 1;
			}
		}

		SDL_PumpEvents();

		// Mouse stuff
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

		// Left click for drawing color
		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, 255);
			DrawCircle(renderer, mouseX, mouseY, Brush.size);
			// Middle click for quitting
		} else if ((buttons & SDL_BUTTON_MMASK) != 0) {
			quit = 1;
			// Right click for 'erasing' - drawing with background color
		} else if ((buttons & SDL_BUTTON_RMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			DrawCircle(renderer, mouseX, mouseY, Brush.size);
		}

		// Draw the UI elements
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
		SDL_RenderFillRect(renderer, &UIrect);

		// The size slider
		slider(renderer, 1, WIDTH - 100.0f - 40.0f, HEIGHT - UIrect.h / 2, 100.0f, &Brush.size, 0, 31);
		// Color sliders
		slider(renderer, 2, 40.0f, HEIGHT - UIrect.h / 2, 127.0f, &Brush.r, 0, 255);
		slider(renderer, 3, 127.0f + 80.0f, HEIGHT - UIrect.h / 2, 127.0f, &Brush.g, 0, 255);
		slider(renderer, 4, 254.0f + 120.0f, HEIGHT - UIrect.h / 2, 127.0f, &Brush.b, 0, 255);

		SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, 255);
		DrawCircle(renderer, 800.0f, HEIGHT - UIrect.h / 2, Brush.size);

		// Present the renderer and wait 1/60th of a second, to get ~60 FPS
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 60);
	}

	// Cleaning up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// Checks if the given x and y is inside the given SDL_Rect
Uint8 RectCheckBounds(int x, int y, struct SDL_Rect Rect) {
	if (x > Rect.x && x < Rect.x + Rect.w && y > Rect.y && y < Rect.y + Rect.h) {
		return 1;
	} else {
		return 0;
	}
}

static void slider(SDL_Renderer *renderer, int id,
		float pos_x, float pos_y, float len,
		Uint8 *value, float min, float max)
{
	// TODO: display the current value of the slider

	// Slider Body
	{
		if (id == 1) {
			SDL_SetRenderDrawColor(renderer, Brush.size * 8, Brush.size * 8, Brush.size * 8, 255);
		} else {
			SDL_SetRenderDrawColor(renderer, Brush.r * (id == 2), Brush.g * (id == 3), Brush.b * (id == 4), 255);
		}
		SDL_Rect rect = {
			.x = pos_x,
			.y = pos_y - SLIDER_THICCNESS * 0.5f,
			.w = len,
			.h = SLIDER_THICCNESS,
		};
		SDL_RenderFillRect(renderer, &rect);
	}

	// Grip
	{
		assert(min <= max);
		float grip_value = ilerpf(min, max, *value) * len;

		// TODO: the grip should go outside of the slider body

		SDL_SetRenderDrawColor(renderer, 255 * (id == 2), 255 * (id == 3), 255 * (id == 4), 255);
		SDL_Rect rect = {
			.x = pos_x - SLIDER_GRIP_SIZE + grip_value,
			.y = pos_y - SLIDER_GRIP_SIZE,
			.w = SLIDER_GRIP_SIZE * 2.0f,
			.h = SLIDER_GRIP_SIZE * 2.0f,
		};
		SDL_RenderFillRect(renderer, &rect);

		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);

		// TODO: the grip should maintain the initial offset between its position and mouse_x

		if (active_id < 0) {
			SDL_Point cursor = {x, y};
			if (SDL_PointInRect(&cursor, &rect) && (buttons & SDL_BUTTON_LMASK) != 0) {
				active_id = id;
			}
		} else {
			if (active_id == id) {
				if ((buttons & SDL_BUTTON_LMASK) == 0) {
					active_id = -1;
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

static inline float clampf(float v, float lo, float hi)
{
	if (v < lo) v = lo;
	if (v > hi) v = hi;
	return v;
}

static inline float lerpf(float a, float b, float t)
{
	return (b - a) * t + a;
}

static inline float ilerpf(float a, float b, float v)
{
	return (v - a) / (b - a);
}

// Function for drawing a circle i stole from somewhere
// and then modified to fill the circle
void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius) {
	int offsetx, offsety, d;
	int status;

	// This little for loop is my addition
	for (; radius > 0; radius--) {
		offsetx = 0;
		offsety = radius;
		d = radius -1;
		status = 0;

		while (offsety >= offsetx) {
			status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
			status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
			status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
			status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
			status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
			status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
			status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
			status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

			if (status < 0) {
				status = -1;
				break;
			}

			if (d >= 2*offsetx) {
				d -= 2*offsetx + 1;
				offsetx +=1;
			}
			else if (d < 2 * (radius - offsety)) {
				d += 2 * offsety - 1;
				offsety -= 1;
			}
			else {
				d += 2 * (offsety - offsetx - 1);
				offsety -= 1;
				offsetx += 1;
			}
		}
	}
}
