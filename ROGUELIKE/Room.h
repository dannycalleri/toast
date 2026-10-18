/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef ROOM_H
#define ROOM_H

#include "Maze.h"
#include "ToastEntity.h"

namespace Toast
{
	class Grid;
	class Tilemap;
	struct RoomTemplate;

	enum DoorState { DOOR_NONE, DOOR_OPEN, DOOR_CLOSED, DOOR_LOCKED };

	class Room : public Entity
	{
	public:
		static const int TILE = 16;
		static const int COLUMNS = 20;
		static const int ROWS = 12;
		static const int WIDTH = COLUMNS * TILE;
		static const int HEIGHT = ROWS * TILE;

		// indices in assets/tiles.png, in the order of art/tiles.txt
		enum Tile
		{
			FLOOR = 0, FLOOR_CRACKED = 1, FLOOR_PEBBLES = 2, FLOOR_MOSS = 3,
			WALL_TOP = 4, WALL_BOTTOM = 5, WALL_LEFT = 6, WALL_RIGHT = 7, WALL_CORNER = 8,
			DOOR_OPEN_TILE = 9, DOOR_CLOSED_TILE = 10, DOOR_LOCKED_TILE = 11,
			INNER_WALL = 12, ROCK = 13, POT = 14, CRATE = 15, BONES = 16, GRASS = 17
		};

		// column and row are the room's cell in the maze grid; the room
		// sits at (column * WIDTH, row * HEIGHT) in the world
		Room(const RoomTemplate& layout, unsigned char doorMask, int column, int row);
		virtual ~Room();

		void setDoor(Direction d, DoorState state);
		DoorState door(Direction d) const { return doors[d]; }

		// room tile coordinates; outside the room counts as solid
		bool isSolid(int column, int row);
		bool isSolidAt(float worldX, float worldY);

		// the middle of the next tile on a shortest walk from one point to another
		// (world coordinates, 4-way steps over tiles that are not solid);
		// false when there is no walk
		bool nextStep(float fromX, float fromY, float toX, float toY, float& stepX, float& stepY);

		// the first of the door's two tiles, in room tile coordinates
		static void doorTile(Direction d, int& column, int& row);

		const RoomTemplate* layout;
		Tilemap* tilemap;
		int column;
		int row;

	private:
		Room(const Room&);
		Room& operator=(const Room&);

		Grid* solid;
		DoorState doors[4];
	};
}

#endif // ROOM_H
