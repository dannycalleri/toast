/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "Maze.h"
#include "RoomTemplates.h"
#include <vector>

namespace Toast
{
	class Room;
	class World;

	// A maze turned into rooms in a world. The world owns the rooms.
	class Level
	{
	public:
		Level();

		bool loadTemplates();
		// false, and nothing built, when the templates did not load
		bool build(World* world, unsigned int seed);
		void clear(World* world);
		// for when the world already deleted the rooms (World::removeAll)
		void forgetRooms();

		// NULL for a cell without a room
		Room* roomAt(int cell) const;
		Room* startRoom() const { return roomAt(maze.start); }

		RoomTemplates templates;
		Maze maze;

	private:
		bool ready;
		std::vector<Room*> rooms;
	};
}

#endif // LEVEL_H
