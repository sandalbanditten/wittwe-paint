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

	// The canvas rectangle
	SDL_Rect CanvasRect = {
		.w = WIDTH,
		.h = HEIGHT - UIrect.h,
		.x = 0,
		.y = 0
	};

	// The button for resseting the canvas
	SDL_Rect resetRect = {
		.w = UIrect.h / 2,
		.h = UIrect.h / 4,
		.x = WIDTH / 2,
		.y = HEIGHT - UIrect.h * 0.75f + UIrect.h / 4
	};

	SDL_Rect resetRect2 = {
		.w = UIrect.h / 2,
		.h = UIrect.h / 4,
		.x = WIDTH / 2,
		.y = HEIGHT - UIrect.h * 0.75f
	};

	SDL_Rect screenshotRect = {
		.w = UIrect.h / 2,
		.h = UIrect.h / 2,
		.x = WIDTH / 2 + UIrect.h,
		.y = HEIGHT - UIrect.h * 0.75f
	};

	SDL_Rect exitRect = {
		.w = UIrect.h / 2,
		.h = UIrect.h / 2,
		.x = WIDTH / 2 + UIrect.h * 2,
		.y = HEIGHT - UIrect.h * 0.75f
	};

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
		Uint32 buttons = SDL_GetMouseState(&Brush.loc.x, &Brush.loc.y);

		// Interacting with UI
		if (buttons & SDL_BUTTON_LMASK) {
			if (SDL_PointInRect(&Brush.loc, &CanvasRect)) {
				SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, 255);
				DrawCircle(renderer, Brush.loc.x, Brush.loc.y, Brush.size, 1);
			} else if (SDL_PointInRect(&Brush.loc, &resetRect)) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderClear(renderer);
			} else if (SDL_PointInRect(&Brush.loc, &resetRect2)) {
				SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, 255);
				SDL_RenderClear(renderer);
			} else if (SDL_PointInRect(&Brush.loc, &screenshotRect)) {
				saveScreenshot("screenshot.bmp", renderer, CanvasRect);
			} else if (SDL_PointInRect(&Brush.loc, &exitRect)) {
				quit = 1;
			}
		} else if (buttons & SDL_BUTTON_RMASK) {
			if (SDL_PointInRect(&Brush.loc, &CanvasRect)) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				DrawCircle(renderer, Brush.loc.x, Brush.loc.y, Brush.size, 1);
			}
		}

		// Draw the UI elements
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
		SDL_RenderFillRect(renderer, &UIrect);

		// Colored button
		SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, 255);
		SDL_RenderFillRect(renderer, &resetRect2);

		// Buttons - First white fill, and then black outline
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &resetRect);
		SDL_RenderFillRect(renderer, &screenshotRect);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &exitRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &resetRect);
		SDL_RenderDrawRect(renderer, &resetRect2);
		SDL_RenderDrawRect(renderer, &screenshotRect);
		SDL_RenderDrawRect(renderer, &exitRect);

		// The size slider
		slider(renderer, 1, WIDTH - 100.0f - 40.0f, HEIGHT - UIrect.h / 2, 100.0f, &Brush.size, 0, 31,
				Brush.size, Brush.r, Brush.g, Brush.b);
		// Color sliders
		slider(renderer, 2, 40.0f, HEIGHT - UIrect.h / 2, 127.0f, &Brush.r, 0, 255,
				Brush.size, Brush.r, Brush.g, Brush.b);
		slider(renderer, 3, 127.0f + 80.0f, HEIGHT - UIrect.h / 2, 127.0f, &Brush.g, 0, 255,
				Brush.size, Brush.r, Brush.g, Brush.b);
		slider(renderer, 4, 254.0f + 120.0f, HEIGHT - UIrect.h / 2, 127.0f, &Brush.b, 0, 255,
				Brush.size, Brush.r, Brush.g, Brush.b);

		// First fill, and then outline
		SDL_SetRenderDrawColor(renderer, Brush.r, Brush.g, Brush.b, 255);
		DrawCircle(renderer, 800.0f, HEIGHT - UIrect.h / 2, Brush.size, 1);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		DrawCircle(renderer, 800.0f, HEIGHT - UIrect.h / 2, Brush.size, 0);

		// Present the renderer and wait two milliseconds
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}

	// Cleaning up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
