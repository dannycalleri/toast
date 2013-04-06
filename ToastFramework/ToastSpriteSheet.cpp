/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastSpriteSheet.h"

namespace Toast
{
	SpriteSheet::SpriteSheet(char* fileName, int width, int height, int tileWidth, int tileHeight)
	{
		this->fileName = fileName;
		this->width = width;
		this->height = height;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
	}

	SpriteSheet::~SpriteSheet()
	{
	}


}