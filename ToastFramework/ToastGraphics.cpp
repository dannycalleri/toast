/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastGraphics.h"
#include "ToastError.h"
#include "ToastEngine.h"
#include "ToastTexture.h"
#include "ToastCanvas.h"
#include "ToastFont.h"

namespace Toast
{
	namespace
	{
		const float RADIANS_TO_DEGREES = 180.0f / SDL_PI_F;

		// Toast angles are radians around the top-left
		// corner, like Allegro's; SDL wants degrees
		void renderTexture(SDL_Texture* tex, const SDL_FRect* src, const SDL_FRect* dst, bool flipped, float angle)
		{
			SDL_Renderer* renderer = TF::engine->renderer;
			if(angle == 0.0f && !flipped)
			{
				SDL_RenderTexture(renderer, tex, src, dst);
			}
			else
			{
				SDL_FPoint pivot = {0.0f, 0.0f};
				SDL_RenderTextureRotated(renderer, tex, src, dst, angle * RADIANS_TO_DEGREES, &pivot,
										 flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
			}
		}
	}

	Graphics::Graphics()
	{
	}

	Graphics::~Graphics()
	{
	}

	void Graphics::Clear(byte r, byte g, byte b, byte a)
	{
		SDL_Renderer* renderer = TF::engine->renderer;
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderClear(renderer);
	}

	void Graphics::Present()
	{
		SDL_RenderPresent(TF::engine->renderer);
	}

	Texture* Graphics::LoadTexture(const std::string& fileName)
	{
		Texture* texture = TF::getTexture(fileName);

		if(!texture)
		{
			SDL_Texture* tex = IMG_LoadTexture(TF::engine->renderer, TF::resolvePath(fileName).c_str());
			if(!tex)
			{
				TOAST_ERROR(SDL_GetError());
				return NULL;
			}

			texture = new Texture();
			texture->tex = tex;

			float w, h;
			SDL_GetTextureSize(tex, &w, &h);
			texture->setWidth(uint32(w));
			texture->setHeight(uint32(h));

			// inserting texture in the map
			// for managing resources
			TF::addTexture(fileName, texture);
		}

		return texture;
	}

	void Graphics::Draw(Texture* source, float destX, float destY, bool flipped, float angle)
	{
		if(!source)
		{
			TOAST_ERROR("[TEXTURE DRAW] The texture is NULL!");
			return;
		}

		// textures are shared, so the alpha left by the last DrawRegion must be reset
		SDL_SetTextureAlphaModFloat(source->tex, 1.0f);
		SDL_FRect dst = {destX, destY, float(source->getWidth()), float(source->getHeight())};
		renderTexture(source->tex, NULL, &dst, flipped, angle);
	}

	void Graphics::DrawRegion(Texture* source, float srcX, float srcY, float srcWidth, float srcHeight, 
							  float destX, float destY, float alpha, bool flipped, float angle)
	{
		DrawRegionScaled(source, srcX, srcY, srcWidth, srcHeight, destX, destY, 1.0f, 1.0f, alpha, flipped, angle);
	}

	void Graphics::DrawRegionScaled(Texture* source, float srcX, float srcY, float srcWidth, float srcHeight, float destX, float destY,
									float scaleX, float scaleY, float alpha, bool flipped, float angle)
	{
		if(!source)
		{
			TOAST_ERROR("[TEXTURE DRAW] The texture is NULL!");
			return;
		}

		SDL_SetTextureAlphaModFloat(source->tex, alpha);
		SDL_FRect src = {srcX, srcY, srcWidth, srcHeight};
		SDL_FRect dst = {destX, destY, srcWidth * scaleX, srcHeight * scaleY};
		renderTexture(source->tex, &src, &dst, flipped, angle);
	}

	void Graphics::DrawCanvas(Canvas* canvas, float destX, float destY)
	{
		if(!canvas || !canvas->getBuffer())
		{
			TOAST_ERROR("[CANVAS DRAW] The canvas is NULL!");
			return;
		}

		SDL_Texture* buffer = canvas->getBuffer();
		// the buffer holds premultiplied colors (see Canvas::init),
		// so the alpha has to scale the color as well
		SDL_SetTextureColorModFloat(buffer, canvas->alpha, canvas->alpha, canvas->alpha);
		SDL_SetTextureAlphaModFloat(buffer, canvas->alpha);
		SDL_FRect dst = {destX, destY, float(canvas->width), float(canvas->height)};
		SDL_RenderTexture(TF::engine->renderer, buffer, NULL, &dst);
	}

	void Graphics::DrawTextFont(Font* font, const std::string& text, byte r, byte g, byte b, float x, float y)
	{
		if(!font || !font->getHandle())
		{
			TOAST_ERROR("[TEXT DRAW] The font is NULL!");
			return;
		}

		if(text.empty())
			return;

		SDL_Color color = {r, g, b, 255};
		SDL_Surface* surface = TTF_RenderText_Blended(font->getHandle(), text.c_str(), 0, color);
		if(!surface)
		{
			TOAST_ERROR(SDL_GetError());
			return;
		}

		SDL_Texture* tex = SDL_CreateTextureFromSurface(TF::engine->renderer, surface);
		if(tex)
		{
			SDL_FRect dst = {x, y, float(surface->w), float(surface->h)};
			SDL_RenderTexture(TF::engine->renderer, tex, NULL, &dst);
			SDL_DestroyTexture(tex);
		}
		SDL_DestroySurface(surface);
	}
}
