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
		al_destroy_font(this->font);
	}
}