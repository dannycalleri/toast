/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastTexture.h"

namespace Toast
{
	Texture::Texture()
	{
		this->width=0;
		this->height=0;
		this->tex=0;
		this->flipped=false;
	}

	Texture::~Texture()
	{
		al_destroy_bitmap(tex);
	}

}