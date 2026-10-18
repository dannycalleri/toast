/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef MAZE_H
#define MAZE_H

#include <random>
#include <vector>

namespace Toast
{
	enum Direction { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

	inline unsigned char doorBit(Direction d) { return (unsigned char)(1 << d); }
	Direction opposite(Direction d);

	// Cells are indexed row by row: cell = row * width + column.
	class Maze
	{
	public:
		static const int DEFAULT_WIDTH = 9;
		static const int DEFAULT_HEIGHT = 7;
		static const int DEFAULT_ROOMS = 12;
		static const int MAX_EXTRA_DOORS = 2;

		struct ExtraDoor
		{
			int cell;
			Direction direction;
		};

		// Returns an empty maze when rooms < 2 or rooms >= width * height.
		static Maze generate(std::mt19937& rng, int rooms = DEFAULT_ROOMS,
							 int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);

		Maze();

		bool empty() const { return rooms.empty(); }
		bool isRoom(int cell) const;
		bool hasDoor(int cell, Direction d) const;
		// -1 when the neighbour is outside the grid
		int neighbour(int cell, Direction d) const;
		int column(int cell) const { return cell % width; }
		int row(int cell) const { return cell / width; }

		int width;
		int height;
		// room cells, in the order the generator carved them
		std::vector<int> rooms;
		// per cell: a mask of doorBit(), 0 for cells without a room
		std::vector<unsigned char> doors;
		// per cell: doors to walk from the start, -1 for cells without a room
		std::vector<int> distance;
		std::vector<ExtraDoor> extraDoors;
		int start;
		int locked;
		int steps;

	private:
		void carve(int cell, Direction d);
		void computeDistances();
		void addExtraDoors(std::mt19937& rng, int reservedLeaf);
		int farthestLeaf() const;
	};
}

#endif // MAZE_H
