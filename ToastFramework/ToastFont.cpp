/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastFont.h"

namespace Toast
{
	Font::Font(const std::string& fileName, int size)
	{
		this->fileName = fileName;
		this->size = size;

		font = al_load_ttf_font(this->fileName.c_str(), this->size, 0);
		if (!font)
		{
			TOAST_ERROR("[FONT] Error while loading ttf file");
		}
	}

	Font::~Font()
	{
		//TOAST_SAFE_DELETE(font);
		al_destroy_font(this->font);
	}
}