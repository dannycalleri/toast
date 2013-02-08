/*
=========================================================================

Toast 2D game framework
Copyright (C)2012 Danny Calleri

Toast 2D game framework is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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