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
			float renderX = this->x - this->originX - TF::camera->x * this->scrollX;
			float renderY = this->y - this->originY - TF::camera->y * this->scrollY;

			if(relative)
			{
				renderX += this->point->x;
				renderY += this->point->y;
			}

			Graphics::Draw(this->texture, renderX, renderY, this->flipped, this->angle);
		}
	}

	void Image::centerOrigin()
	{
		originX = (float)(this->width) * 0.5f;
		originY = (float)(this->height) * 0.5f;
	}
}