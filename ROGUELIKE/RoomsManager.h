/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef ROOMSMANAGER_H
#define ROOMSMANAGER_H

#include <vector>
#include "Door.h"

namespace Toast
{
	class Room;
	class GameWorld;

	class RoomsManager
	{
	public:
		RoomsManager(GameWorld* gameWorld);
		virtual ~RoomsManager();

		void populate();
		Room* loadStart();
		Room* loadBoss();
		Room* getRoom(DoorType type);
		void addRoom(Room* r);
		void destroyRoom(Room* room);

		GameWorld* world;
		std::vector<Room*> rooms;
		std::vector<Room*> uRooms;
		std::vector<Room*> dRooms;
		std::vector<Room*> lRooms;
		std::vector<Room*> rRooms;
		Room* startRoom;
		Room* bossRoom;

	private:

		void loadLevel();

	};
}

#endif // ROOMSMANAGER_H