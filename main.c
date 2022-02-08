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

void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius);

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
	SDL_Rect UIrect;
	UIrect.w = WIDTH;
	UIrect.h = 200;
	UIrect.x = 0;
	UIrect.y = HEIGHT - UIrect.h;
	// The red, green, blue, white and black button
	SDL_Rect RedRect;
	RedRect.w = 100;
	RedRect.h = 100;
	RedRect.x = RedRect.h >> 1;
	RedRect.y = HEIGHT - RedRect.h * 1.5;
	SDL_Rect BlueRect;
	BlueRect.w = 100;
	BlueRect.h = 100;
	BlueRect.x = (BlueRect.h >> 1) + 1.5 * BlueRect.h;
	BlueRect.y = HEIGHT - BlueRect.h * 1.5;
	SDL_Rect GreenRect;
	GreenRect.w = 100;
	GreenRect.h = 100;
	GreenRect.x = (GreenRect.h >> 1) + 3 * GreenRect.h;
	GreenRect.y = HEIGHT - GreenRect.h * 1.5;
	SDL_Rect WhiteRect;
	WhiteRect.w = 100;
	WhiteRect.h = 100;
	WhiteRect.x = (WhiteRect.h >> 1) + 4.5 * WhiteRect.h;
	WhiteRect.y = HEIGHT - WhiteRect.h * 1.5;

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
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_PumpEvents();

		// Mouse stuff
		int mouseX, mouseY;
		int size = 20;      // Diameter of the circle being drawn
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

		// Left click for drawing color
		// Middle click for quitting
		// Right click for 'erasing' - drawing with background color
		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			DrawCircle(renderer, mouseX, mouseY, size);
		} else if ((buttons & SDL_BUTTON_MMASK) != 0) {
			quit = 1;
		} else if ((buttons & SDL_BUTTON_RMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			DrawCircle(renderer, mouseX, mouseY, size);
		}

		// Draw the UI elements
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
		SDL_RenderFillRect(renderer, &UIrect);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &RedRect);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &BlueRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &GreenRect);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &WhiteRect);

		// Present the renderer and wait 1/60th of a second, to get ~60 FPS
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 60);
	}

	// Cleaning up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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
