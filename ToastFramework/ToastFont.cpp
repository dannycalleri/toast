/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastFont.h"
#include "TF.h"

namespace Toast
{
	Font::Font(const std::string& fileName, int size)
	{
		this->fileName = fileName;
		this->size = size;

		font = TTF_OpenFont(TF::resolvePath(this->fileName).c_str(), float(this->size));
		if (!font)
		{
			TOAST_ERROR(SDL_GetError());
		}
	}

	Font::~Font()
	{
		if(font)
			TTF_CloseFont(font);
	}
}
