/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastImage.h"
#include "ToastTexture.h"
#include "ToastGraphics.h"
#include <iostream>

namespace Toast
{
	Image::Image(const std::string& fileName)
	{
		Graphic::Graphic();
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
		{
			if(relative)
			{
				Graphics::Draw(this->texture, this->point->x, this->point->y, this->flipped);
			}
			else
			{
				Graphics::Draw(this->texture, this->x, this->y, this->flipped);
			}
		}
	}
}