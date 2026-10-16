/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastTilemap.h"
#include "ToastGraphics.h"
#include "ToastTexture.h"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace Toast
{
	Tilemap::Tilemap(const std::string& fileName, unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight)
	{
		this->tileset = Graphics::LoadTexture(fileName);
		this->width = width;
		this->height = height;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		this->columns = std::ceil( float(width) / tileWidth );
		this->rows = std::ceil( float(height) / tileHeight );

		this->setColumns = 0;
		this->setCount = 0;
		if(tileset)
		{
			this->setColumns = std::ceil( float(tileset->getWidth()) / tileWidth );
			unsigned int setRows = std::ceil( float(tileset->getHeight()) / tileHeight );
			this->setCount = setColumns * setRows;
		}

		this->tiles.assign(columns * rows, EMPTY);
	}

	Tilemap::Tilemap(const Tilemap& rhs) :
		Graphic()
	{
		active = rhs.active;
		visible = rhs.visible;
		x = rhs.x;
		y = rhs.y;
		width = rhs.width;
		height = rhs.height;
		alpha = rhs.alpha;
		scrollX = rhs.scrollX;
		scrollY = rhs.scrollY;
		relative = rhs.relative;
		flipped = rhs.flipped;
		*point = *rhs.point;

		columns = rhs.columns;
		rows = rhs.rows;
		tileWidth = rhs.tileWidth;
		tileHeight = rhs.tileHeight;
		tileset = rhs.tileset;
		setColumns = rhs.setColumns;
		setCount = rhs.setCount;
		tiles = rhs.tiles;
	}

	Tilemap::~Tilemap()
	{
	}

	void Tilemap::setTile(unsigned int column, unsigned int row, unsigned int index)
	{
		if(setCount == 0)
			return;

		// out-of-range values wrap around, as they always have:
		// ROGUELIKE relies on it for walls at column -1
		index %= setCount;
		column %= columns;
		row %= rows;

		tiles[row * columns + column] = index;
	}

	void Tilemap::visibleRange(float offset, float viewSize, unsigned int tileSize, unsigned int count,
							   unsigned int& first, unsigned int& end)
	{
		float firstTile = std::floor(-offset / tileSize);
		float endTile = std::ceil((viewSize - offset) / tileSize);

		first = (unsigned int)std::min(std::max(firstTile, 0.0f), float(count));
		end = (unsigned int)std::min(std::max(endTile, 0.0f), float(count));
		if(end < first)
			end = first;
	}

	void Tilemap::render()
	{
		if(!tileset)
			return;

		float renderX = this->x - TF::camera->x * this->scrollX;
		float renderY = this->y - TF::camera->y * this->scrollY;

		if(relative)
		{
			renderX += this->point->x;
			renderY += this->point->y;
		}

		unsigned int firstCol, endCol, firstRow, endRow;
		visibleRange(renderX, float(TF::width), tileWidth, columns, firstCol, endCol);
		visibleRange(renderY, float(TF::height), tileHeight, rows, firstRow, endRow);

		for(unsigned int row = firstRow; row < endRow; row++)
		{
			for(unsigned int col = firstCol; col < endCol; col++)
			{
				int index = tiles[row * columns + col];
				if(index == EMPTY)
					continue;

				Graphics::DrawRegion(tileset,
									 float((index % setColumns) * tileWidth), float((index / setColumns) * tileHeight),
									 float(tileWidth), float(tileHeight),
									 renderX + col * tileWidth, renderY + row * tileHeight,
									 alpha, false, 0.0f);
			}
		}
	}

	void Tilemap::loadFromString(const std::string& _str, const std::string& columnSep, const std::string& rowSep)
	{
		std::string str = _str;
		std::vector<std::string> rows;
		std::string tmp;

		while(str.find(rowSep, 0) != std::string::npos)
		{
			size_t pos = str.find(rowSep, 0);
			tmp = str.substr(0, pos);
			rows.push_back(tmp);
			str.erase(0, pos+1);
		}

		// if the string contains more characters
		// it means that the last token is in the string
		// (no whitespace \n after the end of the entire string
		if(str.size() > 0)
		{
			// last token is in the string
			rows.push_back(str);
		}

		//std::cout << "LOADED LEVEL ROWS: " << rows.size() << "\n";

		std::string tmpRow;
		int col = 0;

		for(size_t i=0; i < rows.size(); i++)
		{
			//std::cout << rows[i] << "\n";

			tmpRow = rows[i];
			col = 0;

			while(tmpRow.find(columnSep, 0) != std::string::npos)
			{
				size_t pos = tmpRow.find(columnSep, 0);
				tmp = tmpRow.substr(0, pos);

				//std::cout << tmp << "\n";

				std::istringstream ss(tmp);
				int tileIndex;
				ss >> tileIndex;

				setTile(col, i, tileIndex);

				tmpRow.erase(0, pos+1);

				col++;
			}

			if(tmpRow.size() > 0)
			{
				// last token is in the string
				//std::cout << tmpRow << "\n";

				std::istringstream ss(tmpRow);
				int tileIndex;
				ss >> tileIndex;

				setTile(col, i, tileIndex);
			}

		}


	}
}
