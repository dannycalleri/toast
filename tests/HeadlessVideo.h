// Makes the next Engine run on SDL's dummy video driver with the software
// renderer, so a test needs no display. Override priority beats a stray
// SDL_VIDEO_DRIVER in the environment.

#ifndef HEADLESSVIDEO_H
#define HEADLESSVIDEO_H

#include <SDL3/SDL.h>

inline void useHeadlessVideo()
{
	SDL_SetHintWithPriority(SDL_HINT_VIDEO_DRIVER, "dummy", SDL_HINT_OVERRIDE);
	SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "software", SDL_HINT_OVERRIDE);
}

// The software renderer stores opaque alpha when blending into a render
// target and snaps where a GPU filters, so tests of those need a GPU.
// Probed before an Engine starts, because the Engine exits when SDL cannot init.
inline bool gpuRendererAvailable()
{
	SDL_ResetHint(SDL_HINT_VIDEO_DRIVER);
	SDL_ResetHint(SDL_HINT_RENDER_DRIVER);
	bool available = false;
	if(SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Window* window = SDL_CreateWindow("probe", 16, 16, SDL_WINDOW_HIDDEN);
		SDL_Renderer* renderer = window ? SDL_CreateRenderer(window, NULL) : NULL;
		available = renderer && SDL_strcmp(SDL_GetRendererName(renderer), SDL_SOFTWARE_RENDERER) != 0;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
	return available;
}

#endif
