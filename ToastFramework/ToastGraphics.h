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

		static void DrawCanvas(Canvas* canvas);

		static void DrawTextFont(Font* font, const std::string& text, byte r, byte g, byte b, float x, float y);
	};
}

#endif // TOASTGRAPHICS_H