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

#include "ToastSprite.h"
#include "ToastGraphics.h"
#include "ToastSpriteSheet.h"
#include "ToastTexture.h"

namespace Toast
{
	Sprite::Sprite(char* fileName, int x, int y, int width, int height)
	{
		this->fileName = fileName;
		this->x = x;
		this->y = y;
		this->srcX = 0;
		this->srcY = 0;
		this->width = width;
		this->height = height;
		image = NULL;
		init();
	}

	Sprite::~Sprite()
	{
		//SDL_DestroyTexture(image->tex);
	}

	void Sprite::init()
	{
		//image = Graphics::LoadTexture(this->fileName);
		/*if(!image)
		{
			TOAST_ERROR("ERROR: Error loading sprite image!");
		}*/


	}

	void Sprite::render()
	{
		// rendering sprite on renderSurface
		/*Surface::draw(renderSurface, image, this->x, this->y, 
											this->srcX, this->srcY, 
											this->width, this->height);

		Surface::draw(renderSurface, image, this->x + 80, this->y + 80, 
											this->srcX, this->srcY, 
											this->width, this->height);*/
	}
}