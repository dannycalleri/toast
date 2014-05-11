/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "RoomsManager.h"
#include "Room.h"
#include "Door.h"
#include "GameWorld.h"
#include "TF.h"
#include <iostream>

namespace Toast
{
	RoomsManager::RoomsManager(GameWorld* gameWorld):
		world(gameWorld)
	{
		rooms.clear();
		uRooms.clear();
		dRooms.clear();
		lRooms.clear();
		rRooms.clear();
		startRoom = 0;
		bossRoom = 0;

		std::cout << "[ROOMS MANAGER] created - world = " << world << "\n";

		populate();
	}

	RoomsManager::~RoomsManager()
	{
		// destroy memory here

		TOAST_SAFE_DELETE(startRoom);
		TOAST_SAFE_DELETE(bossRoom);

		// we can destroy all rooms
		// because they are only "blueprints"
		// e.g.: a new copy of each room is returned
		// when requested by the Dungeon
		// and once it's attached to the world,
		// will be destroyed by itself
		for(unsigned int i=0; i < rooms.size(); i++)
		{
			TOAST_SAFE_DELETE(rooms[i]);
		}
	}

	Room* RoomsManager::loadStart()
	{
		Room* room = new Room(*startRoom);
		//Room* room = new Room("rooms/start.oel");
		//TOAST_SAFE_DELETE(startRoom);
		//startRoom = room;
		//startRoom->visible=true;

		//room
		world->add(room);

		/*startRoom->x=10;
		startRoom->y = 100;
		world->add(startRoom);*/

		return room;
	}

	Room* RoomsManager::loadBoss()
	{
		Room* room = new Room(*bossRoom);
		//world->add(room);
		return room;
	}

	void RoomsManager::loadLevel()
	{
		// create rooms and load them into rooms array
		rooms.push_back(new Room("rooms/room.oel", world));
		rooms.push_back(new Room("rooms/room_ud.oel", world));
		rooms.push_back(new Room("rooms/room_lr.oel", world));
		rooms.push_back(new Room("rooms/room_ul.oel", world));
		rooms.push_back(new Room("rooms/room_ur.oel", world));
		rooms.push_back(new Room("rooms/room_dl.oel", world));
		rooms.push_back(new Room("rooms/room_dr.oel", world));

		//world->add(room);
	}

	void RoomsManager::addRoom(Room* r)
	{
		world->add(r);
	}

	void RoomsManager::destroyRoom(Room* r)
	{
		TOAST_SAFE_DELETE(r);
	}

	Room* RoomsManager::getRoom(DoorType type)
	{
		Room* room = NULL;
		int ran = 0;
			
		switch(type)
		{
		case DoorType::UP:
			{
				ran = int(TF::random() * uRooms.size());
				room = new Room(*uRooms[ran]);
				break;
			}
		case DoorType::DOWN:
			{
				//trace("REQUEST A DOWN ROOM FROM A SET WITH SIZE = " + dRooms.length);
						
				ran = int(TF::random() * dRooms.size());
				// return a copy of the room
				// this way we can add more rooms
				// of the same type
				room = new Room(*dRooms[ran]);
						
				break;
			}
		case DoorType::LEFT:
			{
				ran = int(TF::random() * lRooms.size());
				room = new Room(*lRooms[ran]);
				break;
			}
		case DoorType::RIGHT:
			{
				ran = int(TF::random() * rRooms.size());
				room = new Room(*rRooms[ran]);
				break;
			}
		}
			
		return room;
	}

	void RoomsManager::populate()
	{
		std::cout << "[ROOMS MANAGER] populate() \n";

		loadLevel();

		startRoom = new Room("rooms/start.oel", world);
		bossRoom = new Room("rooms/boss.oel", world);

		for(unsigned int i=0; i < rooms.size(); i++)
		{
			Room* room = rooms[i];

			if (room->hasDoor(DoorType::DOWN))
			{
				dRooms.push_back(room);
			}
			if (room->hasDoor(DoorType::UP))
			{
				uRooms.push_back(room);
			}
			if (room->hasDoor(DoorType::LEFT))
			{
				lRooms.push_back(room);
			}
			if (room->hasDoor(DoorType::RIGHT))
			{
				rRooms.push_back(room);
			}
		}
	}
}