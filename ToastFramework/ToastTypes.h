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