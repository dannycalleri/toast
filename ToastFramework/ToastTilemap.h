/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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