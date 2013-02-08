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

#include "ToastTilemap.h"
#include "ToastGraphics.h"
#include "ToastTexture.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

namespace Toast
{
	Tilemap::Tilemap(const std::string& fileName, unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight) :
		Canvas(width,height,false)
	{
		this->tileset = Graphics::LoadTexture(fileName);
		this->width = width;
		this->height = height;
		this->columns = std::ceil( float(width) / tileWidth );
		this->rows = std::ceil( float(height) / tileHeight );
		this->tile = new Rectangle(0, 0, tileWidth, tileHeight);

		std::cout << "TILEMAP COLS: " << this->columns << " ROWS: " << this->rows << "\n";

		this->setColumns = std::ceil( float(tileset->getWidth()) / tile->width );
		this->setRows = std::ceil( float( tileset->getHeight()) / tile->height );
		this->setCount = setColumns * setRows;

		this->point = new Point(0.0f, 0.0f);

		std::cout << "TILESET COLS: " << this->setColumns << " ROWS: " << this->setRows << "\n";
		std::cout << "TILESET COUNT: " << this->setCount << "\n";
	}

	Tilemap::~Tilemap()
	{
		delete this->tile;
		delete this->point;
	}

	void Tilemap::setTile(unsigned int column, unsigned int row, unsigned int index)
	{
		// make sure that index, column and row are inside
		// the bounds
		index %= setCount;
		column %= columns;
		row %= rows;

		// retrieving the right tile in the tileset
		tile->x = (index % setColumns) * tile->width;
		tile->y = (index / setColumns) * tile->height;

		// destination point on the screen buffer
		point->x = column * tile->width;
		point->y = row * tile->height;

		Rectangle rect(tile->x, tile->y, tile->width, tile->height);
		this->drawTextureRegion(this->tileset, &rect, point->x, point->y, false);
		//Graphics::DrawRegion(this->tileset, tile->x, tile->y, tile->width, tile->height, point->x, point->y, false);
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

		for(int i=0; i < rows.size(); i++)
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