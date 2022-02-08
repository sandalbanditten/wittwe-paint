/*
 * Dedicated to honey <3
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define NAME "Wittwe Paint"

#define WIDTH 1280
#define HEIGHT 720

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

void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
Uint8 RectCheckBounds(int x, int y, struct SDL_Rect Rect);

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
		.h = 200,
		.x = 0,
		.y = HEIGHT - 200
	};

	for (int i = 0; i < 4; i++) {
		colorRects[i].w = 100;
		colorRects[i].h = 100;
		colorRects[i].x = (colorRects[i].w >> 1) + 1.5 * i * colorRects[i].w;
		colorRects[i].y = HEIGHT - colorRects[i].h * 1.5;
	}

	// Current settings - should probably be a struct
	Uint8 sizeS = 5;
	Uint8 sizeM = 15;
	Uint8 sizeL = 25;
	Brush.size = sizeL;
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

		// Setting the background color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_PumpEvents();

		// Mouse stuff
		int mouseX, mouseY;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

		// Left click for drawing color
		// Middle click for quitting
		// Right click for 'erasing' - drawing with background color
		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			for (int i = 0; i < 4; i++) {
				if (RectCheckBounds(mouseX, mouseY, colorRects[i])) {
					Brush.r = 255 * (i == 0);
					Brush.g = 255 * (i == 1);
					Brush.b = 255 * (i == 2);
				}
			}
			SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, Brush.a);
			DrawCircle(renderer, mouseX, mouseY, Brush.size);
		} else if ((buttons & SDL_BUTTON_MMASK) != 0) {
			quit = 1;
		} else if ((buttons & SDL_BUTTON_RMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			DrawCircle(renderer, mouseX, mouseY, Brush.size);
		}

		// Draw the UI elements
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
		SDL_RenderFillRect(renderer, &UIrect);
	for (int i = 0; i < 4; i++) {
		SDL_SetRenderDrawColor(renderer,
				255 * (i == 0), 255 * (i == 1), 255 * (i == 2), 255);
		SDL_RenderFillRect(renderer, &colorRects[i]);
	}
		// V Should be CurrentColor, when that is implemented
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		DrawCircle(renderer, WIDTH - 50, HEIGHT - UIrect.h / 2, sizeL);
		DrawCircle(renderer, WIDTH - 100, HEIGHT - UIrect.h / 2, sizeM);
		DrawCircle(renderer, WIDTH - 125, HEIGHT - UIrect.h / 2, sizeS);

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
