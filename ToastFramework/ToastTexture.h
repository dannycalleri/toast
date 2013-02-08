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

#ifndef TOASTTEXTURE_H
#define TOASTTEXTURE_H

#include "ToastCommon.h"

namespace Toast
{
	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		inline uint32 getWidth() { return width; }
		inline uint32 getHeight() { return height; }
		inline void setWidth(uint32 _width) { width = _width; }
		inline void setHeight(uint32 _height) { height = _height; }

	public:

		// Allegro representation of the texture
		ALLEGRO_BITMAP* tex;

		//qqq
		bool flipped;

	protected:
		uint32 width;
		uint32 height;
	};
}

#endif // TOASTTEXTURE_H