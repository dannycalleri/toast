#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "Toast.h"

#include "GameWorld.h"
#include "Level.h"
#include "Player.h"
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
		engine = new Engine(320, 240);
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
		world->newGame(seed);
		CHECK(world->room == level.startRoom());
		// the rooms and the kid
		CHECK(world->entities.size() == level.maze.rooms.size() + 1);

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
	CHECK(world->entities.size() == 1);
	delete engine;
}

TOAST_TEST(roguelike_locked_room_is_behind_a_locked_door)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Level& level = world->level;
	world->newGame(7);

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
	(void)world;
	World other;
	Level empty;
	CHECK(!empty.build(&other, 1));
	CHECK(other.entities.empty());
	delete engine;
}

TOAST_TEST(roguelike_kid_stops_at_a_wall_and_slides_along_it)
{
	Engine* engine;
	startGame(engine);

	// an inner wall cell at room tile (8, 5)
	RoomTemplate walled = layoutWith(RoomTemplates::WALL, 7, 4);
	Room room(walled, 0, 0, 0);
	Player kid(5 * Room::TILE, 5 * Room::TILE + 8);
	kid.room = &room;

	kid.move(100, 0);
	float right = kid.x - kid.originX + kid.width;
	CHECK(right <= 8 * Room::TILE);
	CHECK(right > 8 * Room::TILE - 1);
	CHECK(!kid.overlapsSolid(kid.x, kid.y));

	float before = kid.y;
	kid.move(5, 10);
	CHECK(kid.y == before + 10);
	CHECK(kid.x - kid.originX + kid.width <= 8 * Room::TILE);

	delete engine;
}

TOAST_TEST(roguelike_walking_through_a_door_slides_to_the_next_room)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	unsigned int seed = 1;
	world->newGame(seed);
	while(!world->level.maze.hasDoor(world->level.maze.start, RIGHT))
		world->newGame(++seed);

	Room* start = world->room;
	Room* east = world->level.roomAt(world->level.maze.neighbour(world->level.maze.start, RIGHT));
	CHECK(east != NULL);

	world->player->x = start->x + (Room::COLUMNS - 1) * Room::TILE + 2;
	world->player->y = start->y + Room::HEIGHT / 2 + 4;
	TF::elapsed = 0.05f;
	world->update();
	CHECK(world->sliding());
	CHECK(world->room == east);

	// 0.25 s in: still sliding, the camera between the two rooms
	for(int i = 0; i < 5; i++)
		world->update();
	CHECK(world->sliding());
	CHECK(TF::camera->x > start->x && TF::camera->x < east->x);
	// 0.35 s in: done
	world->update();
	world->update();
	CHECK(!world->sliding());
	CHECK(TF::camera->x == east->x);
	CHECK(TF::camera->y == east->y - GameWorld::HUD_HEIGHT);

	// inside the left door of the new room, on the floor, not on the door
	Player* kid = world->player;
	CHECK(kid->x - kid->originX >= east->x + Room::TILE);
	CHECK(kid->x < east->x + 3 * Room::TILE);
	CHECK(!kid->overlapsSolid(kid->x, kid->y));
	world->update();
	CHECK(world->room == east);
	CHECK(!world->sliding());

	delete engine;
}

TOAST_TEST(roguelike_walking_along_a_wall_past_a_door_stays_in_the_room)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	unsigned int seed = 1;
	world->newGame(seed);
	while(!world->level.maze.hasDoor(world->level.maze.start, RIGHT))
		world->newGame(++seed);
	Room* start = world->room;
	Player* kid = world->player;

	// hitbox flush with the right wall, above the door, then down past it
	kid->x = start->x + (Room::COLUMNS - 1) * Room::TILE - kid->width + kid->originX;
	kid->y = start->y + 4 * Room::TILE;
	TF::elapsed = 0.0f;
	for(int i = 0; i < 4 * Room::TILE; i++)
	{
		kid->move(0, 1);
		world->update();
		CHECK(!world->sliding());
	}
	CHECK(world->room == start);
	CHECK(kid->y > start->y + 7 * Room::TILE);

	// and the bottom wall, left to right past the bottom door when there is one
	kid->x = start->x + 7 * Room::TILE;
	kid->y = start->y + (Room::ROWS - 1) * Room::TILE;
	for(int i = 0; i < 5 * Room::TILE; i++)
	{
		kid->move(1, 0);
		world->update();
		CHECK(!world->sliding());
	}
	CHECK(world->room == start);

	delete engine;
}
