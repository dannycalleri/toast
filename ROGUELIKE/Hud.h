/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef HUD_H
#define HUD_H

#include "Maze.h"
#include <vector>

namespace Toast
{
	class Canvas;
	class Font;
	class GameWorld;
	class Spritemap;
	class Text;

	// Hearts, the key, the cats left and the minimap, in the band above the room.
	class Hud
	{
	public:
		static const int HEARTS = 3;
		// a maze cell on the minimap: a 6x4 block every 7x5 pixels
		static const int CELL_WIDTH = 7;
		static const int CELL_HEIGHT = 5;
		static const int MINIMAP_X = 320 - 8 - Maze::DEFAULT_WIDTH * CELL_WIDTH;
		static const int MINIMAP_Y = 6;

		// adds its graphics to the game's world, which owns them
		Hud(GameWorld* game, Font* font);

		void refresh();

		int minimapDraws;

	private:
		void drawMinimap(const std::vector<int>& cells);

		GameWorld* game;
		Spritemap* hearts[HEARTS];
		Spritemap* keyIcon;
		Text* catsText;
		Canvas* minimap;
		int shownCats;
		std::vector<int> shownCells;
	};
}

#endif // HUD_H
