/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTCOMMON_H
#define TOASTCOMMON_H

/*
*	All the common includes	
*	and classes declaration for the engine.
*	
*	Danny
*/

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "ToastTypes.h"
#include "ToastError.h"
#include "ToastKeycodes.h"

#define TOAST_SAFE_DELETE(x) do { delete (x); (x) = NULL; } while(0)

namespace Toast
{
	class Engine;
	class TFRandom;
	class SpriteSheet; // DEPRECATE
	class World;
	class Texture;
	class Image;
	class Graphic;
	class Anim;
	class Spritemap;
	class Tilemap;
	class Canvas;
	class Entity;
	class Console;
	class Font;
	class Text;
	class Hitbox;
	class Mask;
	class Grid;
}

#endif // TOASTCOMMON_H
