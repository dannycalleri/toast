/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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