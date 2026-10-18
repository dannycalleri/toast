/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef ROOMTEMPLATES_H
#define ROOMTEMPLATES_H

#include "Maze.h"
#include <istream>
#include <string>
#include <vector>

namespace Toast
{
	struct RoomTemplate
	{
		std::string name;
		std::vector<std::string> rows;

		char at(int column, int row) const { return rows[row][column]; }
		bool isSolid(int column, int row) const;
	};

	// The inside of a room, read from rooms/rooms.txt (its header documents the format).
	class RoomTemplates
	{
	public:
		static const int COLUMNS = 18;
		static const int ROWS = 10;
		static const char FLOOR = '.';
		static const char WALL = '#';
		static const char ROCK = 'r';
		static const char POT = 'p';
		static const char CRATE = 'c';
		static const char BONES = 'b';
		static const char GRASS = 'g';
		static const char SPAWN = 'S';

		static bool isKnown(char cell);
		static bool isSolid(char cell);
		// the 2x2 cells just inside the door in direction d
		static void doorLane(Direction d, int& column, int& row);

		// Each returns false when nothing usable was read; a bad template
		// is logged and skipped, the others are kept.
		bool load(const std::string& file);
		bool parse(std::istream& in, const std::string& source);

		const RoomTemplate* find(const std::string& name) const;
		// every template except "start" and "locked"
		std::vector<const RoomTemplate*> fighting() const;

		std::vector<RoomTemplate> templates;
	};
}

#endif // ROOMTEMPLATES_H
