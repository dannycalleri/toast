/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

/*
*	All the common includes	
*	and classes declaration for the engine.
*	
*	Danny
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
//#include <allegro5/allegro_direct3d.h>
#include "ToastTypes.h"
#include "ToastError.h"
#include "ToastKeycodes.h"

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
}