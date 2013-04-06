/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTTEXTURE_H
#define TOASTTEXTURE_H

#include "ToastCommon.h"

namespace Toast
{
	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		inline uint32 getWidth() { return width; }
		inline uint32 getHeight() { return height; }
		inline void setWidth(uint32 _width) { width = _width; }
		inline void setHeight(uint32 _height) { height = _height; }

	public:

		// Allegro representation of the texture
		ALLEGRO_BITMAP* tex;

		//qqq
		bool flipped;

	protected:
		uint32 width;
		uint32 height;
	};
}

#endif // TOASTTEXTURE_H