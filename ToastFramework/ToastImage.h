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