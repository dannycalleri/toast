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

#include "ToastCanvas.h"
#include "ToastGraphics.h"
#include "ToastEngine.h"
#include "ToastText.h"

#include <iostream>

namespace Toast
{
	Canvas::Canvas(unsigned int width, unsigned int height, bool transparent)
	{
		this->width = width;
		this->height = height;
		this->transparent=transparent;
		this->buffer = 0;

		init();
	}

	Canvas::~Canvas()
	{
		al_destroy_bitmap(buffer);
	}

	void Canvas::init()
	{
		buffer = al_create_bitmap(this->width, this->height);
		if(!buffer)
		{
			al_show_native_message_box(TF::engine->display, "Error", "Error", "[CANVAS] Failed to create canvas buffer!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		std::cout << "[Canvas::init()] CLEARING CANVAS\n";
		clear(0,0,0);
	}

	void Canvas::render()
	{
		//std::cout << "[CANVAS] ALPHA: " << this->alpha << "\n";
		Graphics::DrawCanvas(this);
	}

	void Canvas::clear(byte r, byte g, byte b)
	{
		if(al_get_target_bitmap() != this->buffer)
			al_set_target_bitmap(this->buffer);

		if(!transparent)
		{
			Graphics::Clear(r*this->alpha, g*this->alpha, b*this->alpha, int(this->alpha*255));
		}
		else
		{
			Graphics::Clear(0, 0, 0, 0);
		}

		TF::engine->resetRenderingBuffer();
	}

	void Canvas::fill(Rectangle* rect, byte r, byte g, byte b)
	{
		if(al_get_target_bitmap() != this->buffer)
			al_set_target_bitmap(this->buffer);

		al_draw_filled_rectangle(rect->x, rect->y, rect->x + rect->width, rect->y + rect->height, al_map_rgb(r,g,b) );

		TF::engine->resetRenderingBuffer();
	}

	void Canvas::drawRect(Rectangle* rect, byte r, byte g, byte b, float thickness)
	{
		if(al_get_target_bitmap() != this->buffer)
			al_set_target_bitmap(this->buffer);

		al_draw_rectangle(rect->x, rect->y, rect->x + rect->width, rect->y + rect->height, al_map_rgb(r,g,b), thickness);

		TF::engine->resetRenderingBuffer();
	}

	void Canvas::drawTextureRegion(Texture* texture, Rectangle* srcRect, float x, float y, bool flipped)
	{
		if(al_get_target_bitmap() != this->buffer)
			al_set_target_bitmap(this->buffer);

		Graphics::DrawRegion(texture, srcRect->x, srcRect->y, srcRect->width, srcRect->height, x, y, alpha, flipped);

		TF::engine->resetRenderingBuffer();
	}

	void Canvas::drawText(Text* text)
	{
		if(al_get_target_bitmap() != this->buffer)
			al_set_target_bitmap(this->buffer);

		Graphics::DrawTextFont(text->font, text->text, 255, 255, 255, text->x, text->y);

		TF::engine->resetRenderingBuffer();
	}
}