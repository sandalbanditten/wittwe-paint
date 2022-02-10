/* Drawing a filled circle */

#include "circle.h"

// Function for drawing a circle i stole from somewhere
// and then modified to fill the circle
void DrawCircle(SDL_Renderer * renderer, int x, int y, int radius, int fill) {
	int offsetx, offsety, d;
	int status;

	// This little for loop is my addition
	for (; radius >= 0; radius--) {
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
		if (fill == 0) break;
	}
}
