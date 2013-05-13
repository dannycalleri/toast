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
	Text::Text(const std::string& text, bool fixed, int width, int height)
	{
		this->text = text;
		this->tmpText = text;
		// the default framework's font
		this->font = TF::font;
		this->canvas = 0;
		this->isFixed=fixed;
		this->width=width;
		this->height=height;
		
		canvas = new Canvas(this->width, this->height);
		updateCanvas();
	}

	Text::~Text()
	{
		delete canvas;
	}

	void Text::updateCanvas()
	{
		if(!isFixed)
		{
			// recalculate width and height
			this->width = al_get_text_width(font->getAllegro(), this->text.c_str());
			this->height = al_get_font_line_height(font->getAllegro());
			if(canvas != 0)
				delete canvas;
			canvas = new Canvas(this->width, this->height);
		}

		canvas->clear();
		canvas->drawText(this);
	}

	void Text::render()
	{
		if(isFixed)
			updateCanvas();

		canvas->relative = this->relative;
		canvas->point->x = this->point->x;
		canvas->point->y = this->point->y;
		canvas->x = this->x;
		canvas->y = this->y;
		canvas->render();
	}
}