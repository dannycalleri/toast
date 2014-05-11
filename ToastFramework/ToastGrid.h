#ifndef TOASTGRID_H
#define TOASTGRID_H

#include "ToastCommon.h"
#include "ToastHitbox.h"
#include <vector>

namespace Toast
{
	class Grid : public Hitbox
	{
	public:
		Grid(int width, int height, int tileWidth, int tileHeight, float x = 0.0f, float y = 0.0f);
		virtual ~Grid();

		void setTile(int column = 0, int row = 0, bool solid = false);
		void clearTile(int column = 0, int row = 0);
		bool getTile(int column = 0, int row = 0);
		/*void setRect(int column = 0, int row = 0, int width = 1, int height = 1, bool solid = true);
		void clearRect(int column = 0, int row = 0, int width = 1, int height = 1);*/
		void loadFromString(const std::string& _str, const std::string& columnSep = ",", const std::string& rowSep = "\n");

		void printDebug();

	public:
		int columns;
		int rows;
		bool usePositions;

	protected:
		Rectangle* tile;
		std::vector<std::vector<int>> data;
	};
}

#endif // TOASTGRID_H