#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "Toast.h"

#include "GameWorld.h"
#include "Level.h"
#include "Room.h"
#include "RoomTemplates.h"

#include <set>
#include <string>

using namespace Toast;

namespace
{
	GameWorld* startGame(Engine*& engine)
	{
		useHeadlessVideo();
		engine = new Engine(1024, 768);
		GameWorld* world = new GameWorld();
		TF::world = world;
		return world;
	}

	RoomTemplate layoutWith(char cell, int column, int row)
	{
		RoomTemplate layout;
		layout.name = "test";
		layout.rows.assign(RoomTemplates::ROWS, std::string(RoomTemplates::COLUMNS, RoomTemplates::FLOOR));
		layout.rows[row][column] = cell;
		return layout;
	}

	// the solid state of both tiles of a door: 2 both solid, 0 both open
	int solidDoorTiles(Room& room, Direction d)
	{
		int column, row;
		Room::doorTile(d, column, row);
		bool horizontal = d == UP || d == DOWN;
		return room.isSolid(column, row) + room.isSolid(column + (horizontal ? 1 : 0), row + (horizontal ? 0 : 1));
	}
}

TOAST_TEST(roguelike_room_doors_follow_the_mask)
{
	Engine* engine;
	startGame(engine);

	RoomTemplate layout = layoutWith(RoomTemplates::FLOOR, 0, 0);
	Room room(layout, doorBit(UP), 0, 0);
	CHECK(solidDoorTiles(room, UP) == 0);
	CHECK(solidDoorTiles(room, DOWN) == 2);
	CHECK(solidDoorTiles(room, LEFT) == 2);
	CHECK(solidDoorTiles(room, RIGHT) == 2);

	int column, row;
	Room::doorTile(UP, column, row);
	CHECK(room.isSolid(column - 1, row));
	CHECK(room.isSolid(column + 2, row));
	CHECK(!room.isSolid(column, row + 1));

	room.setDoor(UP, DOOR_CLOSED);
	CHECK(solidDoorTiles(room, UP) == 2);
	room.setDoor(UP, DOOR_OPEN);
	CHECK(solidDoorTiles(room, UP) == 0);

	delete engine;
}

TOAST_TEST(roguelike_rocks_are_solid_and_bones_are_not)
{
	Engine* engine;
	startGame(engine);

	RoomTemplate rock = layoutWith(RoomTemplates::ROCK, 4, 3);
	Room rocky(rock, 0, 2, 1);
	CHECK(rocky.isSolid(5, 4));
	CHECK(!rocky.isSolid(6, 4));
	CHECK(rocky.isSolidAt(2 * Room::WIDTH + 5 * Room::TILE + 1, 1 * Room::HEIGHT + 4 * Room::TILE + 1));
	CHECK(!rocky.isSolidAt(2 * Room::WIDTH + 6 * Room::TILE + 1, 1 * Room::HEIGHT + 4 * Room::TILE + 1));

	RoomTemplate bones = layoutWith(RoomTemplates::BONES, 4, 3);
	Room bony(bones, 0, 0, 0);
	CHECK(!bony.isSolid(5, 4));

	delete engine;
}

TOAST_TEST(roguelike_maze_rooms_use_several_templates)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Level& level = world->level;
	CHECK(!level.templates.templates.empty());

	for(unsigned int seed = 1; seed <= 20; seed++)
	{
		CHECK(level.build(world, seed));
		CHECK(world->entities.size() == level.maze.rooms.size());

		std::set<std::string> fighting;
		for(size_t i = 0; i < level.maze.rooms.size(); i++)
		{
			int cell = level.maze.rooms[i];
			Room* room = level.roomAt(cell);
			CHECK(room != NULL);
			CHECK(room->x == level.maze.column(cell) * Room::WIDTH);
			CHECK(room->y == level.maze.row(cell) * Room::HEIGHT);
			if(cell == level.maze.start)
				CHECK(room->layout->name == "start");
			else if(cell == level.maze.locked)
				CHECK(room->layout->name == "locked");
			else
				fighting.insert(room->layout->name);
		}
		CHECK(fighting.size() >= 3);
	}

	level.clear(world);
	CHECK(world->entities.empty());
	delete engine;
}

TOAST_TEST(roguelike_locked_room_is_behind_a_locked_door)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Level& level = world->level;
	CHECK(level.build(world, 7));

	const Maze& maze = level.maze;
	int doors = 0;
	for(int d = UP; d <= RIGHT; d++)
	{
		if(!maze.hasDoor(maze.locked, (Direction)d))
			continue;
		Room* outside = level.roomAt(maze.neighbour(maze.locked, (Direction)d));
		CHECK(outside->door(opposite((Direction)d)) == DOOR_LOCKED);
		CHECK(level.roomAt(maze.locked)->door((Direction)d) == DOOR_OPEN);
		doors++;
	}
	CHECK(doors == 1);
	delete engine;
}

TOAST_TEST(roguelike_level_without_templates_builds_nothing)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Level empty;
	CHECK(!empty.build(world, 1));
	CHECK(world->entities.empty());
	delete engine;
}
