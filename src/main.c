/*
 * Dedicated to honey <3
 */

#include "main.h"

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

	for (int i = 0; i < COLORS; i++) {
		colorRects[i].w = 80;
		colorRects[i].h = 80;
		colorRects[i].x = i * colorRects[i].w + 10 * i + 10;
		colorRects[i].y = HEIGHT - colorRects[i].h - 10;
	}

	// Fill the array of different available sizes
	for (int i = 0; i < SIZES; i++) {
		sizes[i] = 5 + 10 * i;
	}

	Brush.size = sizes[4];
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
			// Loops through the different color rectangles
			// if they are clicked on, sets the brush to the apropriate color
			for (int i = 0; i < COLORS; i++) {
				if (RectCheckBounds(mouseX, mouseY, colorRects[i])) {
					Brush.r = 255 * (i == 0);
					Brush.g = 255 * (i == 1);
					Brush.b = 255 * (i == 2);
				}
			}
			for (int i = 0; i < SIZES; i++) {
				if (CircleCheckBounds(mouseX, mouseY, sizes[i])) {
					Brush.size = sizes[i];
				}
			}
			SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, Brush.a);
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

		// Draw the color options
		for (int i = 0; i < COLORS; i++) {
			SDL_SetRenderDrawColor(renderer,
					255 * (i == 0), 255 * (i == 1), 255 * (i == 2), 255);
			SDL_RenderFillRect(renderer, &colorRects[i]);
		}

		// Draws the circles for choosing size
		SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, Brush.a);
		for (int i = 0; i < SIZES; i++) {
			DrawCircle(renderer, WIDTH - 500 + 100 * i, HEIGHT - UIrect.h / 2, sizes[i]);
		}

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

// Checking if a point is inside a circle, using pythagores
Uint8 CircleCheckBounds(int x, int y, Uint8 d) {
	if (sqrt((float)(x * x + y * y)) < d) {
		return 1;
	} else {
		return 0;
	}
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
