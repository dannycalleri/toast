/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastCanvas.h"
#include "ToastGraphics.h"
#include "ToastEngine.h"
#include "ToastText.h"

namespace Toast
{
	namespace
	{
		class TargetScope
		{
		public:
			TargetScope(SDL_Texture* target)
			{
				renderer = TF::engine->renderer;
				previous = SDL_GetRenderTarget(renderer);
				SDL_SetRenderTarget(renderer, target);
			}

			~TargetScope()
			{
				SDL_SetRenderTarget(renderer, previous);
			}

		private:
			SDL_Renderer* renderer;
			SDL_Texture* previous;
		};
	}

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
		if(buffer)
			SDL_DestroyTexture(buffer);
	}

	void Canvas::init()
	{
		// SDL cannot create a 0x0 texture (an empty Text has width 0)
		int w = width > 0 ? width : 1;
		int h = height > 0 ? height : 1;
		buffer = SDL_CreateTexture(TF::engine->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		if(!buffer)
		{
			TOAST_ERROR("[CANVAS] Failed to create canvas buffer!");
			return;
		}

		// blending into a transparent target leaves premultiplied colors
		SDL_SetTextureBlendMode(buffer, SDL_BLENDMODE_BLEND_PREMULTIPLIED);

		clear(0,0,0);
	}

	void Canvas::render()
	{
		float renderX = this->x - TF::camera->x * this->scrollX;
		float renderY = this->y - TF::camera->y * this->scrollY;

		if(relative) 
		{
			renderX += this->point->x;
			renderY += this->point->y;
		}

		Graphics::DrawCanvas(this, renderX, renderY);
	}

	void Canvas::clear(byte r, byte g, byte b)
	{
		if(!buffer) return;
		TargetScope scope(buffer);

		if(!transparent)
		{
			Graphics::Clear(r*this->alpha, g*this->alpha, b*this->alpha, int(this->alpha*255));
		}
		else
		{
			Graphics::Clear(0, 0, 0, 0);
		}
	}

	void Canvas::fill(Rectangle* rect, byte r, byte g, byte b)
	{
		if(!buffer) return;
		TargetScope scope(buffer);

		SDL_Renderer* renderer = TF::engine->renderer;
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_FRect area = {rect->x, rect->y, rect->width, rect->height};
		SDL_RenderFillRect(renderer, &area);
	}

	void Canvas::drawRect(Rectangle* rect, byte r, byte g, byte b, float thickness)
	{
		if(!buffer) return;
		TargetScope scope(buffer);

		SDL_Renderer* renderer = TF::engine->renderer;
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);

		// rect sizes can be negative (the console's selection rect
		// dragged up or left), so normalise before drawing the edges
		float x = rect->width < 0 ? rect->x + rect->width : rect->x;
		float y = rect->height < 0 ? rect->y + rect->height : rect->y;
		float w = SDL_fabsf(rect->width);
		float h = SDL_fabsf(rect->height);
		float t = thickness;

		// SDL has no thick outline, so draw it as four bands
		// centred on the edges, as Allegro did
		SDL_FRect edges[4] = {
			{x - t/2, y - t/2, w + t, t},
			{x - t/2, y + h - t/2, w + t, t},
			{x - t/2, y - t/2, t, h + t},
			{x + w - t/2, y - t/2, t, h + t}
		};
		SDL_RenderFillRects(renderer, edges, 4);
	}

	void Canvas::drawTextureRegion(Texture* texture, Rectangle* srcRect, float x, float y, bool flipped)
	{
		if(!buffer) return;
		TargetScope scope(buffer);

		Graphics::DrawRegion(texture, srcRect->x, srcRect->y, srcRect->width, srcRect->height, x, y, alpha, flipped, 0.0f);
	}

	void Canvas::drawText(Text* text)
	{
		if(!buffer) return;
		TargetScope scope(buffer);

		// the canvas is exactly the text's size, so text->x/y
		// (where the canvas goes) would push the glyphs off it
		Graphics::DrawTextFont(text->font, text->text, 255, 255, 255, 0.0f, 0.0f);
	}
}
