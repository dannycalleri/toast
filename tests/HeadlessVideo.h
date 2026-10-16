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

#endif
