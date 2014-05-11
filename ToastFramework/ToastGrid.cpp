#include "ToastGrid.h"
#include <iostream>
#include <sstream>

namespace Toast
{
	Grid::Grid(int width, int height, int tileWidth, int tileHeight, float x, float y):
		Hitbox(width, height, x, y)
	{
		if (!width || !height || !tileWidth || !tileHeight) TOAST_ERROR("[GRID] Illegal: sizes cannot be 0.");


		//float f = ceilf((float)width / tileWidth);

		columns = ceilf((float)width / tileWidth);
		rows = ceilf((float)height / tileHeight);
		
		// resize vector to fit the data
		data.resize(rows);
		for(unsigned int i=0; i < data.size(); i++)
			data[i].resize(columns);

		tile = new Rectangle(0, 0, tileWidth, tileHeight);
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->usePositions=false;
	}

	Grid::~Grid()
	{
		delete tile;
	}

	void Grid::setTile(int column, int row, bool solid)
	{
		if(usePositions)
		{
			column /= tile->width;
			row /= tile->height;
		}

		data[row][column] = solid;
	}

	void Grid::clearTile(int column, int row)
	{
		if(usePositions)
		{
			column /= tile->width;
			row /= tile->height;
		}

		setTile(column, row, false);
	}

	bool Grid::getTile(int column, int row)
	{
		if(usePositions)
		{
			column /= tile->width;
			row /= tile->height;
		}

		return data[row][column];
	}

	void Grid::loadFromString(const std::string& _str, const std::string& columnSep, const std::string& rowSep)
	{
		std::string str = _str;
		std::vector<std::string> rows;
		std::string tmp;

		// get all the rows
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

			if(columnSep != "")
			{
				// scan a column
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
			}
			else // reading bitstring
			{
				for(unsigned int j=0; j < tmpRow.length(); j++)
				{
					char bitChar = tmpRow[j];
					int bit = bitChar-'0';
					setTile(j, i, bit);

					//std::cout << a << " ";
				}

				//std::cout << "\n";
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

	void Grid::printDebug()
	{
		std::cout << "[GRID] printDebug(): \n";

		for(unsigned int i=0; i < data.size(); i++)
		{
			for(unsigned int j=0; j < data[i].size(); j++)
			{
				std::cout << data[i][j];
			}

			std::cout << "\n";
		}
	}
}