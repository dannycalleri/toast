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

#ifndef TOASTTEXT_H
#define TOASTTEXT_H

#include "ToastCommon.h"
#include "ToastGraphic.h"
#include <string>

namespace Toast
{
	class Text : public Graphic
	{
	public:
		Text(const std::string& text="");
		virtual ~Text();

		virtual void update() {};
		virtual void render();

		Font* font;
		std::string text;

	private:
		
		void updateCanvas();
		std::string tmpText; // text copy to optimize rendering

		// a canvas in which 
		// to draw the text
		Canvas* canvas;
	};
}

#endif // TOASTTEXT_H