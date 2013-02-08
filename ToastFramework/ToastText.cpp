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

#include "ToastText.h"
#include "ToastFont.h"
#include "ToastGraphics.h"
#include "ToastCanvas.h"
#include <iostream>

namespace Toast
{
	Text::Text(const std::string& text)
	{
		this->text = text;
		this->tmpText = text;
		// the default framework's font
		this->font = TF::font;

		this->width = al_get_text_width(font->getAllegro(), this->text.c_str());
		this->height = al_get_font_line_height(font->getAllegro());

		//std::cout << "TEXT WIDTH: " << this->width << "\n";
		//std::cout << "TEXT HEIGHT: " << this->height << "\n";

		canvas = new Canvas(this->width, this->height);
		updateCanvas();
	}

	Text::~Text()
	{
		delete canvas;
	}

	void Text::updateCanvas()
	{
		canvas->drawText(this);
	}

	void Text::render()
	{
		canvas->x = this->x;
		canvas->y = this->y;
		canvas->render();
	}
}