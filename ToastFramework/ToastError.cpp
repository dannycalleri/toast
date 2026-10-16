/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastError.h"

#include <SDL3/SDL.h>

namespace Toast
{
	void reportError(const char* msg)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", msg);
	}

	void fatalError(const char* msg)
	{
		reportError(msg);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Toast Error", msg, NULL);
	}
}
