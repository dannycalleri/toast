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

#ifndef TOASTTILEMAP_H
#define TOASTTILEMAP_H

#include "ToastCommon.h"
//#include "ToastGraphic.h"
#include "ToastCanvas.h"
#include <string>

namespace Toast
{
	class Tilemap : public Canvas
	{
	public:
		Tilemap(const std::string& fileName, unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight);
		virtual ~Tilemap();

		void setTile(unsigned int column, unsigned int row, unsigned int index = 0);
		void loadFromString(const std::string& _str, const std::string& columnSep = ",", const std::string& rowSep = "\n");


	private:

		// Tilemap data - width and height of the level
		unsigned int width;
		unsigned int height;
		unsigned int columns;
		unsigned int rows;

		// Tileset data - width, height of the tileset file
		Texture* tileset;
		unsigned int setRows;
		unsigned int setColumns;
		unsigned int setCount;

		Rectangle* tile;

		// destination point on
		// the rendering buffer
		Point* point;

	};
}

#endif // TOASTTILEMAP_H