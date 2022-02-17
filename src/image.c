#include "image.h"

int saveScreenshot(const char* location, SDL_Renderer *renderer, SDL_Rect area) {
	SDL_Surface *surface = NULL;

	// Create SDLsurface with depth of 32 bits
	surface = SDL_CreateRGBSurface(0, area.w, area.h, 32, 0, 0, 0, 0);
	if (surface == NULL) {
		return 1;
	}

	// Get data from SDL_Renderer and save them into surface
	if (SDL_RenderReadPixels(renderer, &area, surface->format->format, surface->pixels, surface->pitch) != 0) {
		SDL_FreeSurface(surface);
		return 1;
	}

	// Save screenshot as PNG file
	if (SDL_SaveBMP(surface, location) != 0) {
		SDL_FreeSurface(surface);
		return 1;
	}

	SDL_FreeSurface(surface);
	return 1;
}
