/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTIMAGE_H
#define TOASTIMAGE_H

#include "ToastCommon.h"
#include "ToastGraphic.h"
#include <string>

namespace Toast
{
	class Image : public Graphic
	{
	public:
		Image(const std::string& fileName = "");
		virtual ~Image();

		virtual void update() {};
		virtual void render();

		void centerOrigin();

	public:
		float angle;
		float scale;
		float scaleX;
		float scaleY;
		float originX;
		float originY;
		//bool flipped;

	//protected:
		Texture* texture;
	};
}

#endif // TOASTIMAGE_H