/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Level.h"
#include "Room.h"
#include "ToastError.h"
#include "ToastWorld.h"
#include <random>
#include <utility>

namespace Toast
{
	Level::Level() :
		ready(false)
	{
	}

	bool Level::loadTemplates()
	{
		templates = RoomTemplates();
		ready = false;
		if(!templates.load("rooms/rooms.txt") || !templates.find("start") ||
		   !templates.find("locked") || templates.fighting().empty())
		{
			TOAST_ERROR("[LEVEL] rooms/rooms.txt needs a start room, a locked room and at least one other room");
			return false;
		}
		ready = true;
		return true;
	}

	bool Level::build(World* world, unsigned int seed)
	{
		clear(world);
		if(!ready)
			return false;

		std::mt19937 rng(seed);
		maze = Maze::generate(rng);
		rooms.assign(maze.width * maze.height, NULL);

		// dealt like cards, so a maze repeats a layout only after using them all
		std::vector<const RoomTemplate*> deck = templates.fighting();
		for(int i = (int)deck.size() - 1; i > 0; i--)
			std::swap(deck[i], deck[rng() % (unsigned int)(i + 1)]);

		size_t dealt = 0;
		for(size_t i = 0; i < maze.rooms.size(); i++)
		{
			int cell = maze.rooms[i];
			const RoomTemplate* layout;
			if(cell == maze.start)
				layout = templates.find("start");
			else if(cell == maze.locked)
				layout = templates.find("locked");
			else
				layout = deck[dealt++ % deck.size()];

			Room* room = new Room(*layout, maze.doors[cell], maze.column(cell), maze.row(cell));
			rooms[cell] = room;
			world->add(room);
		}

		for(int d = UP; d <= RIGHT; d++)
		{
			Room* outside = roomAt(maze.neighbour(maze.locked, (Direction)d));
			if(outside && maze.hasDoor(maze.locked, (Direction)d))
				outside->setDoor(opposite((Direction)d), DOOR_LOCKED);
		}
		return true;
	}

	void Level::clear(World* world)
	{
		for(size_t i = 0; i < rooms.size(); i++)
		{
			if(rooms[i])
				world->remove(rooms[i]);
		}
		rooms.clear();
	}

	Room* Level::roomAt(int cell) const
	{
		if(cell < 0 || cell >= (int)rooms.size())
			return NULL;
		return rooms[cell];
	}
}
