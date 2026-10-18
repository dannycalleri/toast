#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "Toast.h"

#include "Dungeon.h"
#include "GameWorld.h"
#include "Room.h"
#include "RoomsManager.h"
#include "ToastTilemap.h"

#include <cstdio>
#include <fstream>

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
}

TOAST_TEST(roguelike_every_room_file_loads_with_its_doors)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	struct Expected { const char* file; bool up, down, left, right; };
	const Expected rooms[] = {
		{"rooms/room.oel",    true,  true,  true,  true},
		{"rooms/room_ud.oel", true,  true,  false, false},
		{"rooms/room_lr.oel", false, false, true,  true},
		{"rooms/room_ul.oel", true,  false, true,  false},
		{"rooms/room_ur.oel", true,  false, false, true},
		{"rooms/room_dl.oel", false, true,  true,  false},
		{"rooms/room_dr.oel", false, true,  false, true},
		{"rooms/boss.oel",    false, true,  false, false},
		{"rooms/start.oel",   true,  false, false, false},
	};

	for(const Expected& e : rooms)
	{
		Room room(e.file, world);
		if(!room.tilemap || room.box->width <= 0 || room.box->height <= 0)
		{
			ToastTest::fail(__FILE__, __LINE__, e.file);
			continue;
		}
		CHECK(room.hasDoor(UP) == e.up);
		CHECK(room.hasDoor(DOWN) == e.down);
		CHECK(room.hasDoor(LEFT) == e.left);
		CHECK(room.hasDoor(RIGHT) == e.right);
	}

	delete engine;
}

TOAST_TEST(roguelike_missing_room_file_gives_an_empty_room)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	Room room("rooms/missing.oel", world);
	CHECK(room.tilemap == NULL);
	CHECK(room.doors.empty());
	CHECK(room.box->width == 0 && room.box->height == 0);

	delete engine;
}

TOAST_TEST(roguelike_dungeons_generate_without_overlapping_rooms)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	for(int run = 0; run < 25; run++)
	{
		world->dungeon->destroy();
		CHECK(world->dungeon->generate(10));

		CHECK(world->dungeon->rooms.size() >= 9);
		for(Room* room : world->dungeon->rooms)
			CHECK(!room->isOverlapping());
	}

	delete engine;
}

TOAST_TEST(roguelike_room_copies_do_not_reread_the_room_file)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	std::string path = TF::resolvePath("rooms/copy-test.oel");
	{
		std::ifstream in(TF::resolvePath("rooms/room_ud.oel").c_str(), std::ios::binary);
		std::ofstream out(path.c_str(), std::ios::binary);
		out << in.rdbuf();
	}
	Room original("rooms/copy-test.oel", world);
	std::remove(path.c_str());

	Room copy(original);
	CHECK(copy.tilemap != NULL && copy.tilemap != original.tilemap);
	CHECK(copy.doors.size() == original.doors.size());
	CHECK(copy.box->x == original.box->x && copy.box->width == original.box->width);
	CHECK(copy.hasDoor(UP) && copy.hasDoor(DOWN));

	delete engine;
}

TOAST_TEST(roguelike_generation_gives_up_when_no_room_can_be_placed)
{
	Engine* engine;
	GameWorld* world = startGame(engine);

	// what RoomsManager is left with when the room files fail to load
	world->roomsManager->uRooms.clear();
	world->roomsManager->dRooms.clear();
	world->roomsManager->lRooms.clear();
	world->roomsManager->rRooms.clear();

	CHECK(!world->dungeon->generate(10));
	CHECK(world->dungeon->rooms.empty());
	CHECK(world->entities.empty());

	delete engine;
}
