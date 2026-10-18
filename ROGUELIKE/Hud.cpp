/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Hud.h"
#include "Effects.h"
#include "GameWorld.h"
#include "Player.h"
#include "Room.h"
#include "TF.h"
#include "ToastCanvas.h"
#include "ToastTypes.h"
#include "ToastSpritemap.h"
#include "ToastText.h"
#include <string>

namespace Toast
{
	namespace
	{
		enum Cell { HIDDEN, KNOWN, VISITED, CURRENT, LOCKED };

		Spritemap* icon(GameWorld* game, float x, float y)
		{
			Spritemap* sprite = new Spritemap("assets/fx.png", 8, 8);
			int full[1] = {Effects::HEART};
			int half[1] = {Effects::HALF_HEART};
			int empty[1] = {Effects::EMPTY_HEART};
			int key[1] = {Effects::KEY};
			sprite->add("full", full, 1);
			sprite->add("half", half, 1);
			sprite->add("empty", empty, 1);
			sprite->add("key", key, 1);
			sprite->x = x;
			sprite->y = y;
			sprite->scrollX = 0;
			sprite->scrollY = 0;
			game->addGraphic(sprite);
			return sprite;
		}
	}

	Hud::Hud(GameWorld* game, Font* font) :
		minimapDraws(0),
		game(game),
		shownCats(-1)
	{
		for(int i = 0; i < HEARTS; i++)
			hearts[i] = icon(game, 8.0f + i * 10, 8.0f);
		keyIcon = icon(game, 8.0f, 20.0f);
		keyIcon->play("key");

		catsText = new Text("");
		catsText->font = font;
		catsText->x = 8;
		catsText->y = 32;
		catsText->scrollX = 0;
		catsText->scrollY = 0;
		game->addGraphic(catsText);

		minimap = new Canvas(Maze::DEFAULT_WIDTH * CELL_WIDTH, Maze::DEFAULT_HEIGHT * CELL_HEIGHT, false);
		minimap->x = MINIMAP_X;
		minimap->y = MINIMAP_Y;
		minimap->scrollX = 0;
		minimap->scrollY = 0;
		game->addGraphic(minimap);
	}

	void Hud::refresh()
	{
		Player* player = game->player;
		for(int i = 0; i < HEARTS; i++)
		{
			int health = player ? player->health - i * 2 : 0;
			hearts[i]->play(health >= 2 ? "full" : health == 1 ? "half" : "empty");
		}
		keyIcon->visible = game->hasKey;

		if(game->catsLeft != shownCats)
		{
			shownCats = game->catsLeft;
			catsText->text = "CATS LEFT " + std::to_string(shownCats);
			catsText->updateCanvas();
		}

		const Maze& maze = game->level.maze;
		std::vector<int> cells(maze.width * maze.height, HIDDEN);
		for(size_t i = 0; i < maze.rooms.size(); i++)
		{
			int cell = maze.rooms[i];
			if(!game->visited(cell))
				continue;
			cells[cell] = VISITED;
			for(int d = UP; d <= RIGHT; d++)
			{
				int next = maze.neighbour(cell, (Direction)d);
				if(maze.hasDoor(cell, (Direction)d) && cells[next] == HIDDEN)
					cells[next] = KNOWN;
			}
		}
		if(cells[maze.locked] != HIDDEN)
			cells[maze.locked] = LOCKED;
		if(game->room)
			cells[game->room->row * maze.width + game->room->column] = CURRENT;

		if(cells != shownCells)
		{
			shownCells = cells;
			drawMinimap(cells);
		}
	}

	void Hud::drawMinimap(const std::vector<int>& cells)
	{
		minimapDraws++;
		minimap->clear(0x1a, 0x1c, 0x2c);
		const Maze& maze = game->level.maze;
		for(size_t cell = 0; cell < cells.size(); cell++)
		{
			Rectangle block((float)(maze.column((int)cell) * CELL_WIDTH), (float)(maze.row((int)cell) * CELL_HEIGHT),
							CELL_WIDTH - 1, CELL_HEIGHT - 1);
			switch(cells[cell])
			{
			case KNOWN:   minimap->fill(&block, 0x56, 0x6c, 0x86); break;
			case VISITED: minimap->fill(&block, 0x94, 0xb0, 0xc2); break;
			case CURRENT: minimap->fill(&block, 0xf4, 0xf4, 0xf4); break;
			case LOCKED:  minimap->fill(&block, 0xff, 0xcd, 0x75); break;
			default:      break;
			}
		}
	}
}
