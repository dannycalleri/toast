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

#include "ToastImage.h"
#include "ToastTexture.h"
#include "ToastGraphics.h"

namespace Toast
{
	Image::Image(const std::string& fileName)
	{
		this->texture=0;
		if(fileName != "")
			this->texture = Graphics::LoadTexture(fileName);
		this->width = this->texture->getWidth();
		this->height = this->texture->getHeight();
		this->angle = 0.0f;
		this->scale = 0.0f;
		this->scaleX = 0.0f;
		this->scaleY = 0.0f;
		this->originX = 0.0f;
		this->originY = 0.0f;
		//this->flipped = false;
	}

	Image::~Image()
	{
	}

	void Image::render()
	{
		if(texture)
			Graphics::Draw(this->texture, this->x, this->y, this->flipped);
	}
}