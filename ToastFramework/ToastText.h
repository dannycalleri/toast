/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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