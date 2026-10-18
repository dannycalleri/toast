/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Room.h"
#include "RoomTemplates.h"
#include "ToastGrid.h"
#include "ToastTilemap.h"
#include <cmath>

namespace Toast
{
	namespace
	{
		int propTile(char cell)
		{
			switch(cell)
			{
			case RoomTemplates::WALL:  return Room::INNER_WALL;
			case RoomTemplates::ROCK:  return Room::ROCK;
			case RoomTemplates::POT:   return Room::POT;
			case RoomTemplates::CRATE: return Room::CRATE;
			case RoomTemplates::BONES: return Room::BONES;
			case RoomTemplates::GRASS: return Room::GRASS;
			default:                   return -1;
			}
		}

		// a fixed scatter, so a room looks the same every time it is built
		int floorTile(int column, int row, int roomColumn, int roomRow)
		{
			int noise = (column * 31 + row * 17 + roomColumn * 7 + roomRow * 13) % 11;
			return noise < 3 ? Room::FLOOR_CRACKED + noise : Room::FLOOR;
		}

		int wallTile(int column, int row)
		{
			bool left = column == 0;
			bool right = column == Room::COLUMNS - 1;
			bool top = row == 0;
			bool bottom = row == Room::ROWS - 1;
			if((left || right) && (top || bottom))
				return Room::WALL_CORNER;
			if(top)
				return Room::WALL_TOP;
			if(bottom)
				return Room::WALL_BOTTOM;
			return left ? Room::WALL_LEFT : Room::WALL_RIGHT;
		}
	}

	Room::Room(const RoomTemplate& layout, unsigned char doorMask, int column, int row) :
		Entity(column * WIDTH, row * HEIGHT),
		layout(&layout),
		column(column),
		row(row)
	{
		tilemap = new Tilemap("assets/tiles.png", WIDTH, HEIGHT, TILE, TILE);
		solid = new Grid(WIDTH, HEIGHT, TILE, TILE);

		for(int r = 0; r < ROWS; r++)
		{
			for(int c = 0; c < COLUMNS; c++)
			{
				bool wall = r == 0 || c == 0 || r == ROWS - 1 || c == COLUMNS - 1;
				if(wall)
				{
					tilemap->setTile(c, r, wallTile(c, r));
					solid->setTile(c, r, true);
					continue;
				}

				char cell = layout.at(c - 1, r - 1);
				int prop = propTile(cell);
				tilemap->setTile(c, r, prop >= 0 ? prop : floorTile(c, r, column, row));
				solid->setTile(c, r, RoomTemplates::isSolid(cell));
			}
		}

		for(int d = UP; d <= RIGHT; d++)
		{
			doors[d] = DOOR_NONE;
			if(doorMask & doorBit((Direction)d))
				setDoor((Direction)d, DOOR_OPEN);
		}

		graphic = tilemap;
	}

	Room::~Room()
	{
		delete solid;
	}

	void Room::doorTile(Direction d, int& column, int& row)
	{
		switch(d)
		{
		case UP:    column = COLUMNS / 2 - 1; row = 0; break;
		case DOWN:  column = COLUMNS / 2 - 1; row = ROWS - 1; break;
		case LEFT:  column = 0; row = ROWS / 2 - 1; break;
		default:    column = COLUMNS - 1; row = ROWS / 2 - 1; break;
		}
	}

	void Room::setDoor(Direction d, DoorState state)
	{
		doors[d] = state;

		int column, row;
		doorTile(d, column, row);
		for(int i = 0; i < 2; i++)
		{
			int c = column + (d == UP || d == DOWN ? i : 0);
			int r = row + (d == LEFT || d == RIGHT ? i : 0);
			switch(state)
			{
			case DOOR_NONE:   tilemap->setTile(c, r, wallTile(c, r)); break;
			case DOOR_OPEN:   tilemap->setTile(c, r, DOOR_OPEN_TILE); break;
			case DOOR_CLOSED: tilemap->setTile(c, r, DOOR_CLOSED_TILE); break;
			case DOOR_LOCKED: tilemap->setTile(c, r, DOOR_LOCKED_TILE); break;
			}
			solid->setTile(c, r, state != DOOR_OPEN);
		}
	}

	bool Room::isSolid(int column, int row)
	{
		if(column < 0 || row < 0 || column >= COLUMNS || row >= ROWS)
			return true;
		return solid->getTile(column, row);
	}

	bool Room::isSolidAt(float worldX, float worldY)
	{
		return isSolid((int)std::floor((worldX - x) / TILE), (int)std::floor((worldY - y) / TILE));
	}
}
