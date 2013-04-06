/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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