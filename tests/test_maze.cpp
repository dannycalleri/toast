#include "ToastTest.h"

#include "Maze.h"

#include <chrono>

using namespace Toast;

namespace
{
	int doorCount(const Maze& maze, int cell)
	{
		int count = 0;
		for(int d = UP; d <= RIGHT; d++)
			count += maze.hasDoor(cell, (Direction)d);
		return count;
	}
}

TOAST_TEST(maze_has_the_requested_rooms_all_reachable)
{
	for(unsigned int seed = 1; seed <= 200; seed++)
	{
		std::mt19937 rng(seed);
		Maze maze = Maze::generate(rng);
		CHECK(maze.rooms.size() == (size_t)Maze::DEFAULT_ROOMS);
		CHECK(maze.start == (Maze::DEFAULT_HEIGHT / 2) * Maze::DEFAULT_WIDTH + Maze::DEFAULT_WIDTH / 2);

		int reachable = 0;
		for(size_t cell = 0; cell < maze.distance.size(); cell++)
			reachable += maze.distance[cell] >= 0;
		CHECK(reachable == Maze::DEFAULT_ROOMS);
	}
}

TOAST_TEST(maze_doors_are_symmetric_and_lead_to_rooms)
{
	for(unsigned int seed = 1; seed <= 200; seed++)
	{
		std::mt19937 rng(seed);
		Maze maze = Maze::generate(rng);
		for(int cell = 0; cell < maze.width * maze.height; cell++)
		{
			for(int d = UP; d <= RIGHT; d++)
			{
				if(!(maze.doors[cell] & doorBit((Direction)d)))
					continue;
				CHECK(maze.isRoom(cell));
				int next = maze.neighbour(cell, (Direction)d);
				CHECK(next >= 0);
				CHECK(maze.isRoom(next));
				CHECK(maze.hasDoor(next, opposite((Direction)d)));
			}
		}
	}
}

TOAST_TEST(maze_same_seed_gives_the_same_maze)
{
	std::mt19937 a(1234);
	std::mt19937 b(1234);
	Maze first = Maze::generate(a);
	Maze second = Maze::generate(b);
	CHECK(first.rooms == second.rooms);
	CHECK(first.doors == second.doors);
	CHECK(first.start == second.start);
	CHECK(first.locked == second.locked);

	std::mt19937 c(4321);
	CHECK(Maze::generate(c).doors != first.doors);
}

TOAST_TEST(maze_locked_room_is_the_farthest_dead_end)
{
	for(unsigned int seed = 1; seed <= 500; seed++)
	{
		std::mt19937 rng(seed);
		Maze maze = Maze::generate(rng);
		CHECK(maze.locked >= 0);
		CHECK(maze.locked != maze.start);
		CHECK(doorCount(maze, maze.locked) == 1);
		for(size_t i = 0; i < maze.rooms.size(); i++)
		{
			int cell = maze.rooms[i];
			if(cell != maze.start && doorCount(maze, cell) == 1)
				CHECK(maze.distance[cell] <= maze.distance[maze.locked]);
		}
	}

	std::mt19937 rng(7);
	Maze two = Maze::generate(rng, 2);
	CHECK(two.locked >= 0 && two.locked != two.start);
}

TOAST_TEST(maze_extra_doors_avoid_the_start_and_the_locked_room)
{
	int added = 0;
	for(unsigned int seed = 1; seed <= 500; seed++)
	{
		std::mt19937 rng(seed);
		Maze maze = Maze::generate(rng);
		CHECK(maze.extraDoors.size() <= (size_t)Maze::MAX_EXTRA_DOORS);
		for(size_t i = 0; i < maze.extraDoors.size(); i++)
		{
			const Maze::ExtraDoor& door = maze.extraDoors[i];
			int other = maze.neighbour(door.cell, door.direction);
			CHECK(door.cell != maze.start && other != maze.start);
			CHECK(door.cell != maze.locked && other != maze.locked);
			CHECK(maze.hasDoor(door.cell, door.direction));
		}
		added += (int)maze.extraDoors.size();
	}
	// loops must actually happen, not only be allowed
	CHECK(added > 0);
}

TOAST_TEST(maze_generation_takes_at_most_two_steps_per_room)
{
	for(unsigned int seed = 1; seed <= 200; seed++)
	{
		std::mt19937 rng(seed);
		Maze maze = Maze::generate(rng);
		CHECK(maze.steps <= 2 * Maze::DEFAULT_ROOMS);
	}

	// a grid with exactly one spare cell leaves the generator the fewest choices
	std::mt19937 rng(99);
	Maze tight = Maze::generate(rng, 24, 5, 5);
	CHECK(tight.rooms.size() == 24);
	CHECK(tight.steps <= 2 * 24);
}

TOAST_TEST(maze_of_ten_thousand_rooms_is_fast)
{
	std::mt19937 rng(42);
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Maze maze = Maze::generate(rng, 10000, 200, 200);
	double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - begin).count();
	CHECK(maze.rooms.size() == 10000);
	CHECK(maze.steps <= 20000);
	CHECK(ms < 50.0);
}

TOAST_TEST(maze_invalid_sizes_give_an_empty_maze)
{
	std::mt19937 rng(1);
	CHECK(Maze::generate(rng, 1).empty());
	CHECK(Maze::generate(rng, 0).empty());
	CHECK(Maze::generate(rng, 63, 9, 7).empty());
	CHECK(Maze::generate(rng, 100, 9, 7).empty());
	CHECK(Maze::generate(rng, 62, 9, 7).rooms.size() == 62);
}
