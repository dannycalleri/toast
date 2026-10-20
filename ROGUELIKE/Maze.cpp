/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Maze.h"
#include <utility>

namespace Toast
{
	namespace
	{
		// rng() % n instead of std::uniform_int_distribution, whose output
		// differs between standard libraries: a seed gives one maze everywhere
		int randomBelow(std::mt19937& rng, int n)
		{
			return (int)(rng() % (unsigned int)n);
		}

		int doorCount(unsigned char mask)
		{
			int count = 0;
			for(; mask; mask >>= 1)
				count += mask & 1;
			return count;
		}
	}

	Direction opposite(Direction d)
	{
		switch(d)
		{
		case UP:    return DOWN;
		case DOWN:  return UP;
		case LEFT:  return RIGHT;
		default:    return LEFT;
		}
	}

	Maze::Maze() :
		width(0),
		height(0),
		start(-1),
		locked(-1),
		steps(0)
	{
	}

	bool Maze::isRoom(int cell) const
	{
		return cell >= 0 && cell < (int)doors.size() && distance[cell] >= 0;
	}

	bool Maze::hasDoor(int cell, Direction d) const
	{
		return isRoom(cell) && (doors[cell] & doorBit(d)) != 0;
	}

	int Maze::neighbour(int cell, Direction d) const
	{
		int col = column(cell);
		int r = row(cell);
		switch(d)
		{
		case UP:    return r > 0 ? cell - width : -1;
		case DOWN:  return r < height - 1 ? cell + width : -1;
		case LEFT:  return col > 0 ? cell - 1 : -1;
		default:    return col < width - 1 ? cell + 1 : -1;
		}
	}

	void Maze::carve(int cell, Direction d)
	{
		doors[cell] |= doorBit(d);
		doors[neighbour(cell, d)] |= doorBit(opposite(d));
	}

	Maze Maze::generate(std::mt19937& rng, int roomCount, int width, int height)
	{
		Maze maze;
		if(roomCount < 2 || width < 1 || height < 1 || roomCount >= width * height)
			return maze;

		maze.width = width;
		maze.height = height;
		maze.doors.assign(width * height, 0);
		std::vector<bool> visited(width * height, false);

		maze.start = (height / 2) * width + width / 2;
		visited[maze.start] = true;
		maze.rooms.push_back(maze.start);

		std::vector<int> active(1, maze.start);
		while((int)maze.rooms.size() < roomCount)
		{
			maze.steps++;

			int index = randomBelow(rng, 2) ? (int)active.size() - 1 : randomBelow(rng, (int)active.size());
			int cell = active[index];

			Direction order[4] = {UP, DOWN, LEFT, RIGHT};
			for(int i = 3; i > 0; i--)
				std::swap(order[i], order[randomBelow(rng, i + 1)]);

			bool carved = false;
			for(int i = 0; i < 4 && !carved; i++)
			{
				int next = maze.neighbour(cell, order[i]);
				if(next >= 0 && !visited[next])
				{
					maze.carve(cell, order[i]);
					visited[next] = true;
					maze.rooms.push_back(next);
					active.push_back(next);
					carved = true;
				}
			}

			if(!carved)
			{
				active[index] = active.back();
				active.pop_back();
			}
		}

		maze.computeDistances();
		maze.addExtraDoors(rng, maze.farthestLeaf());
		maze.computeDistances();
		maze.locked = maze.farthestLeaf();
		return maze;
	}

	void Maze::computeDistances()
	{
		distance.assign(width * height, -1);
		std::vector<int> queue;
		queue.reserve(rooms.size());
		queue.push_back(start);
		distance[start] = 0;

		for(size_t head = 0; head < queue.size(); head++)
		{
			int cell = queue[head];
			for(int d = UP; d <= RIGHT; d++)
			{
				if(!(doors[cell] & doorBit((Direction)d)))
					continue;
				int next = neighbour(cell, (Direction)d);
				if(distance[next] < 0)
				{
					distance[next] = distance[cell] + 1;
					queue.push_back(next);
				}
			}
		}
	}

	int Maze::farthestLeaf() const
	{
		int best = -1;
		for(size_t i = 0; i < rooms.size(); i++)
		{
			int cell = rooms[i];
			if(cell == start || doorCount(doors[cell]) != 1)
				continue;
			if(best < 0 || distance[cell] > distance[best])
				best = cell;
		}
		return best;
	}

	void Maze::addExtraDoors(std::mt19937& rng, int reservedLeaf)
	{
		// the reserved leaf stays a leaf, so a locked room always exists afterwards;
		// a door added here gives both its rooms two doors, so neither can become it
		std::vector<ExtraDoor> candidates;
		for(size_t i = 0; i < rooms.size(); i++)
		{
			int cell = rooms[i];
			const Direction ahead[2] = {DOWN, RIGHT};
			for(int k = 0; k < 2; k++)
			{
				int next = neighbour(cell, ahead[k]);
				if(next < 0 || distance[next] < 0 || (doors[cell] & doorBit(ahead[k])))
					continue;
				if(cell == start || next == start || cell == reservedLeaf || next == reservedLeaf)
					continue;
				candidates.push_back(ExtraDoor{cell, ahead[k]});
			}
		}

		for(int added = 0; added < MAX_EXTRA_DOORS && !candidates.empty(); added++)
		{
			int pick = randomBelow(rng, (int)candidates.size());
			carve(candidates[pick].cell, candidates[pick].direction);
			extraDoors.push_back(candidates[pick]);
			candidates[pick] = candidates.back();
			candidates.pop_back();
		}
	}
}
