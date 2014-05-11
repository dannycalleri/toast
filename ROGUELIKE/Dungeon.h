/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include "Room.h"

namespace Toast
{
	class Room;
	class GameWorld;

	/*struct distanceCompare
	{
		inline int operator()(const Room* a, const Room* b)
		{
			if (a->distance < b->distance) return 1;
			else if (a->distance > b->distance) return -1;
			else return 0;
		}
	};*/

	class Dungeon
	{
	public:
		Dungeon(GameWorld* gameWorld);
		virtual ~Dungeon();

		/**
		*	Generate a new random dungeon of the specified size
		*/
		void generate(int size);
		void destroy();

		unsigned int size;
		std::vector<Room*> rooms;

	private:

		// creating rooms for the 
		// current dungeon
		void createRooms(unsigned int size);

		// calculates distance
		// from the startRoom to
		// all other rooms
		void calculateDistance();

		// finds where to place
		// the boss' room
		void findBossSpot();

		// closes opened doors
		// and cleans the dungeon
		void closeDoors();

		// loads rooms' content
		void loadRooms();

		void buildRoom(Room* room);

		void createBossDoor(Room* r, int startCol);

		static bool distanceCompare(const Room* a, const Room* b);

		GameWorld* world;
		int tries;
		const int MAX_TRIES;
		int cycles;
		int maxCycles;

		// reference to the first room
		// of the dungeon
		Room* startRoom;
	};
}

#endif