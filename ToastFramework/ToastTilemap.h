/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTTILEMAP_H
#define TOASTTILEMAP_H

#include "ToastCommon.h"
#include "ToastGraphic.h"
#include <string>
#include <vector>

namespace Toast
{
	class Tilemap : public Graphic
	{
	public:
		Tilemap(const std::string& fileName, unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight);
		Tilemap(const Tilemap& rhs);
		virtual ~Tilemap();

		void setTile(unsigned int column, unsigned int row, unsigned int index = 0);
		void loadFromString(const std::string& _str, const std::string& columnSep = ",", const std::string& rowSep = "\n");

		virtual void render();

		// the tiles [first, end) of a row or column of count tiles that
		// overlap a view of viewSize pixels, when the map starts at offset
		static void visibleRange(float offset, float viewSize, unsigned int tileSize, unsigned int count,
								 unsigned int& first, unsigned int& end);

	private:
		Tilemap& operator=(const Tilemap&);

		unsigned int columns;
		unsigned int rows;
		unsigned int tileWidth;
		unsigned int tileHeight;

		Texture* tileset;
		unsigned int setColumns;
		unsigned int setCount;

		// tileset index per cell, row by row; EMPTY where no tile was set
		std::vector<int> tiles;
		static constexpr int EMPTY = -1;
	};
}

#endif // TOASTTILEMAP_H
