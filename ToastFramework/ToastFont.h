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

#ifndef TOASTFONT_H
#define TOASTFONT_H

#include "ToastCommon.h"
#include <string>

namespace Toast
{
	class Font
	{
	public:
		Font(const std::string& fileName, int size);
		virtual ~Font();

		inline ALLEGRO_FONT* getAllegro() { return font; }

		int size;

	private:
		// interal font representation
		ALLEGRO_FONT* font;
		// font filename
		std::string fileName;
	};
}

#endif // TOASTFONT_H