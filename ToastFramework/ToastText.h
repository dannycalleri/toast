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
		/*
			When you use fixed = true you must specify
			width and height for this Graphic explicitly.
			Fixed is just an optimization and tells 
			that the Graphic doesn't change width
			or height at runtime. This is useful for rendering
			text that frequently changes (FPS counter).
			In this way updateCanvas() doesn't reallocate memory
			for this Text when it's called.
		*/
		Text(const std::string& text="", bool fixed=false, int width = 0, int height = 0);
		virtual ~Text();

		void updateCanvas();
		virtual void update() {};
		virtual void render();

		Font* font;
		std::string text;
		bool isFixed;

	private:
		std::string tmpText; // text copy to optimize rendering

		// a canvas in which 
		// to draw the text
		Canvas* canvas;
	};
}

#endif // TOASTTEXT_H