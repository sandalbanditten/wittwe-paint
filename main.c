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

	int quit = 0;
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
					quit = 1;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_PumpEvents();

		int mouseX, mouseY;
		int size = 20;
		Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			DrawCircle(renderer, mouseX, mouseY, size);
		} else if ((buttons & SDL_BUTTON_MMASK) != 0) {
			quit = 1;
		} else if ((buttons & SDL_BUTTON_RMASK) != 0) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			DrawCircle(renderer, mouseX, mouseY, size);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius) {
	int offsetx, offsety, d;
	int status;

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
