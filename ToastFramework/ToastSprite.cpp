/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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