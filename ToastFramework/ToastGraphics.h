/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTGRAPHICS_H
#define TOASTGRAPHICS_H

#include "ToastCommon.h"
#include <allegro5/allegro_image.h>
#include "TF.h"
#include <string>
#include <map>

namespace Toast
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		/**
		*	Clears the current target buffer
		*/
		static void Clear(byte r, byte g, byte b, byte a=255);
		static void Present();

		static Texture* LoadTexture(const std::string& fileName);
		static void Draw(Texture* source, float destX, float destY, bool flipped);
		static void DrawRegion(Texture* source, float srcX, float srcY, float srcWidth, float srcHeight, float destX, float destY, float alpha, bool flipped);

		static void DrawCanvas(Canvas* canvas, float destX, float destY);

		static void DrawTextFont(Font* font, const std::string& text, byte r, byte g, byte b, float x, float y);
	};
}

#endif // TOASTGRAPHICS_H