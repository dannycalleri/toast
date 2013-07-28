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

// debug
#include <iostream>

namespace Toast
{
	Graphics::Graphics()
	{
	}

	Graphics::~Graphics()
	{
	}

	void Graphics::Clear(byte r, byte g, byte b, byte a)
	{
		// clearing window with a new color
		al_clear_to_color(al_map_rgba(r, g, b,a));
	}

	void Graphics::Present()
	{
		// present buffer
		al_flip_display();
	}

	Texture* Graphics::LoadTexture(const std::string& fileName)
	{
		Texture* texture = TF::getTexture(fileName);

		std::cout << "[TEXTURE] Loading texture \" " << fileName << "\" \n";

		if(!texture)
		{
			std::cout << "[TEXTURE] Creating NEW texture for \" " << fileName << "\" \n";

			texture = new Texture();
			texture->tex = al_load_bitmap(fileName.c_str());

			if(!texture->tex)
			{
				TOAST_ERROR("[TEXTURE] ERROR loading allegro texture");
				return NULL;
			}

			texture->setWidth(al_get_bitmap_width(texture->tex));
			texture->setHeight(al_get_bitmap_height(texture->tex));

			// inserting texture in the map
			// for managing resources
			TF::addTexture(fileName, texture);
		}

		return texture;
	}

	void Graphics::Draw(Texture* source, float destX, float destY, bool flipped)
	{
		if(!source)
		{
			al_show_native_message_box(TF::engine->display, "Error", "Error", "[TEXTURE DRAW] The texture is NULL!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if(!flipped)
			al_draw_bitmap(source->tex, destX, destY, 0);
		else
			al_draw_bitmap(source->tex, destX, destY, ALLEGRO_FLIP_HORIZONTAL);
	}

	void Graphics::DrawRegion(Texture* source, float srcX, float srcY, float srcWidth, float srcHeight, 
							  float destX, float destY, float alpha, bool flipped)
	{
		if(!source)
		{
			al_show_native_message_box(TF::engine->display, "Error", "Error", "[TEXTURE DRAW] The texture is NULL!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		if(!flipped)
		{
			al_draw_tinted_bitmap_region(source->tex, al_map_rgba_f(1.0f * alpha, 1.0f * alpha, 1.0f * alpha, alpha), 
										 srcX, srcY, srcWidth, srcHeight, destX, destY, 0);
		}
		else
		{
			al_draw_tinted_bitmap_region(source->tex, al_map_rgba_f(1.0f * alpha, 1.0f * alpha, 1.0f * alpha, alpha), 
										 srcX, srcY, srcWidth, srcHeight, destX, destY, ALLEGRO_FLIP_HORIZONTAL);
		}
	}

	void Graphics::DrawCanvas(Canvas* canvas, float destX, float destY)
	{
		if(!canvas)
		{
			al_show_native_message_box(TF::engine->display, "Error", "Error", "[CANVAS DRAW] The canvas is NULL!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		//std::cout << "[CANVAS] ALPHA: " << canvas->alpha << "\n";
		//al_draw_bitmap(canvas->getBuffer(), canvas->x, canvas->x, 0);
		//al_draw_tinted_bitmap(canvas->getBuffer(), al_map_rgba_f(1.0f, 1.0f, 1.0f, canvas->alpha), canvas->x, canvas->x, 0);

		al_draw_tinted_bitmap(canvas->getBuffer(), 
							al_map_rgba_f(1.0f * canvas->alpha, 1.0f * canvas->alpha, 1.0f * canvas->alpha, canvas->alpha), 
							destX, destY, 0);
	}

	void Graphics::DrawTextFont(Font* font, const std::string& text, byte r, byte g, byte b, float x, float y)
	{
		if(!font)
		{
			al_show_native_message_box(TF::engine->display, "Error", "Error", "[TEXT DRAW] The font is NULL!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return;
		}

		al_draw_text(font->getAllegro(), al_map_rgb(r, g, b), x, y, 0, text.c_str());
	}
}