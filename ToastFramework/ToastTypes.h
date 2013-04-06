/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTTYPES_H
#define TOASTTYPES_H

namespace Toast
{
	typedef unsigned __int16 uint16;
	typedef unsigned __int32 uint32;
	typedef unsigned char byte;

	struct Point
	{
		float x, y;
		Point() { x = y = 0.0f; }
		Point(float _x, float _y) { x = _x; y = _y; }
	};

	struct Rectangle
	{
		float x;
		float y;
		float width;
		float height;

		Rectangle() { x = y = width = height = 0.0f; }
		Rectangle(float _x, float _y, float _width, float _height) { x=_x; y=_y; width=_width; height=_height; }
	};
}

#endif