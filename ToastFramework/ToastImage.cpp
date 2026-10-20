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
		this->texture=0;
		if(fileName != "")
			this->texture = Graphics::LoadTexture(fileName);
		if(this->texture)
		{
			this->width = this->texture->getWidth();
			this->height = this->texture->getHeight();
		}
		this->angle = 0.0f;
		this->scale = 1.0f;
		this->scaleX = 1.0f;
		this->scaleY = 1.0f;
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

			Graphics::DrawRegionScaled(this->texture, 0.0f, 0.0f, (float)this->texture->getWidth(), (float)this->texture->getHeight(),
									   renderX, renderY, this->scale * this->scaleX, this->scale * this->scaleY,
									   this->alpha, this->flipped, this->angle);
		}
	}

	void Image::centerOrigin()
	{
		originX = (float)(this->width) * 0.5f;
		originY = (float)(this->height) * 0.5f;
	}
}