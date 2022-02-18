#ifndef IMAGE_H_
#define IMAGE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void saveScreenshot(const char* location, SDL_Renderer *renderer, SDL_Rect area);

#endif
