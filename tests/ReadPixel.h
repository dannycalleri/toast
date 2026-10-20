// Reads back what the renderer drew at a game (logical) pixel.

#ifndef READPIXEL_H
#define READPIXEL_H

#include <SDL3/SDL.h>

// The window opens at a whole multiple of the game's size, so one game pixel
// covers several output pixels; this reads the output pixel at its centre.
inline SDL_Color readGamePixel(SDL_Renderer* renderer, int x, int y)
{
	float windowX, windowY;
	SDL_RenderCoordinatesToWindow(renderer, x + 0.5f, y + 0.5f, &windowX, &windowY);
	float density = SDL_GetWindowPixelDensity(SDL_GetRenderWindow(renderer));

	int logicalWidth, logicalHeight;
	SDL_RendererLogicalPresentation mode;
	SDL_GetRenderLogicalPresentation(renderer, &logicalWidth, &logicalHeight, &mode);
	SDL_SetRenderLogicalPresentation(renderer, 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);

	SDL_Color color = {0, 0, 0, 0};
	SDL_Rect rect = {int(windowX * density), int(windowY * density), 1, 1};
	SDL_Surface* surface = SDL_RenderReadPixels(renderer, &rect);
	if(surface)
	{
		SDL_ReadSurfacePixel(surface, 0, 0, &color.r, &color.g, &color.b, &color.a);
		SDL_DestroySurface(surface);
	}

	SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight, mode);
	return color;
}

#endif
