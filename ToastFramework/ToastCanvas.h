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

#ifndef TOASTCANVAS_H
#define TOASTCANVAS_H

#include "ToastCommon.h"
#include "ToastGraphic.h"

namespace Toast
{
	class Canvas : public Graphic
	{
	public:
		Canvas(unsigned int width, unsigned int height, bool transparent=true);
		virtual ~Canvas();

		inline ALLEGRO_BITMAP* getBuffer() { return buffer; }

		// for rendering canvas on the target
		// buffer
		virtual void render();

		// methods for rendering on canvas

		// clears all canvas
		void clear(byte r=0, byte g=0, byte b=0);
		// fill a region of the canvas with the specified color
		void fill(Rectangle* rect, byte r, byte g, byte b);
		void drawRect(Rectangle* rect, byte r, byte g, byte b, float thickness);
		void drawTextureRegion(Texture* texture, Rectangle* srcRect, float x, float y, bool flipped);
		void drawText(Text* text);

	private:

		bool transparent;

		void init();

		ALLEGRO_BITMAP* buffer; // rendering buffer for this canvas
	};
}

#endif // TOASTCANVAS_H