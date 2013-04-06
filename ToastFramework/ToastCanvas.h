/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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