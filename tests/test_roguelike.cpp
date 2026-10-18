#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "Toast.h"

#include "Bullet.h"
#include "Cat.h"
#include "GameWorld.h"
#include "Key.h"
#include "Level.h"
#include "Player.h"
#include "Room.h"
#include "RoomTemplates.h"
#include "ScreenShake.h"
#include "ToastEmitter.h"
#include "ToastInput.h"
#include "ToastText.h"

#include <algorithm>
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

namespace
{
	std::vector<Bullet*> bulletsIn(World* world)
	{
		std::vector<Bullet*> bullets;
		for(size_t i = 0; i < world->entities.size(); i++)
		{
			Bullet* bullet = dynamic_cast<Bullet*>(world->entities[i]);
			if(bullet)
				bullets.push_back(bullet);
		}
		return bullets;
	}

	void releaseInput()
	{
		Input::leftMouseDown = false;
		for(int key = 0; key < TOAST_KEY_MAX; key++)
			Input::downKeys[key] = false;
	}
}

TOAST_TEST(roguelike_holding_fire_spits_bullets_from_the_mouth_at_the_cooldown_rate)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Player* kid = world->player;

	// the cursor 50 px straight above the kid, in game coordinates
	Input::leftMouseDown = true;
	Input::mouseX = (int)(kid->x - TF::camera->x);
	Input::mouseY = (int)(kid->y - 6 - TF::camera->y) - 50;
	TF::elapsed = 0.05f;
	world->update();

	std::vector<Bullet*> bullets = bulletsIn(world);
	CHECK(bullets.size() == 1);
	CHECK(SDL_fabsf(bullets[0]->x - kid->x) < 1.0f);
	// it already moved this frame: its start is travelled px below it, at the mouth
	CHECK(SDL_fabsf(bullets[0]->y + bullets[0]->travelled - (kid->y - 6)) < 0.5f);
	CHECK(bullets[0]->velocityY < 0 && SDL_fabsf(bullets[0]->velocityX) < 1.0f);
	CHECK(world->effects->particleCount() > 0);
	CHECK(kid->facing == UP);

	for(int i = 1; i < 20; i++)
		world->update();
	CHECK(kid->shots == 4 || kid->shots == 5);

	releaseInput();
	Input::downKeys[TOAST_KEY_LEFT] = true;
	for(int i = 0; i < 6; i++)
		world->update();
	releaseInput();
	bullets = bulletsIn(world);
	CHECK(bullets.back()->velocityX < 0 && SDL_fabsf(bullets.back()->velocityY) < 1.0f);
	CHECK(kid->facing == LEFT);
	delete engine;
}

TOAST_TEST(roguelike_bullet_breaks_on_a_wall_into_particles)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Room* room = world->room;
	world->player->x = room->x + 60;

	// heading for the right wall, 54 px away
	float wallX = room->x + (Room::COLUMNS - 1) * Room::TILE;
	Bullet* bullet = new Bullet(wallX - 54, room->y + 3 * Room::TILE + 8, 1, 0, room, world->effects);
	world->add(bullet);
	int particlesBefore = world->effects->particleCount();

	TF::elapsed = 0.05f;
	int frames = 0;
	while(!bulletsIn(world).empty() && frames < 20)
	{
		world->update();
		frames++;
	}
	CHECK(bulletsIn(world).empty());
	// 54 px at 180 px/s is 0.3 s: it broke on the wall, not at the end of its range
	CHECK(frames == 6 || frames == 7);
	CHECK(world->effects->particleCount() >= particlesBefore + 6);
	delete engine;
}

TOAST_TEST(roguelike_bullet_breaks_at_the_end_of_its_range)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Room* room = world->room;

	Bullet* bullet = new Bullet(room->x + 2 * Room::TILE, room->y + 4 * Room::TILE + 8, 1, 0, room, world->effects);
	world->add(bullet);
	TF::elapsed = 0.05f;
	int frames = 0;
	while(!bulletsIn(world).empty() && frames < 40)
	{
		world->update();
		frames++;
	}
	CHECK(bulletsIn(world).empty());
	// 150 px at 180 px/s: about 0.83 s
	CHECK(frames >= 16 && frames <= 18);
	delete engine;
}

TOAST_TEST(roguelike_new_game_removes_the_bullets_of_the_old_maze)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Input::downKeys[TOAST_KEY_RIGHT] = true;
	TF::elapsed = 0.05f;
	world->update();
	releaseInput();
	CHECK(bulletsIn(world).size() == 1);

	world->newGame(99);
	CHECK(bulletsIn(world).empty());
	CHECK(world->entities.size() == world->level.maze.rooms.size() + 1);
	world->update();
	delete engine;
}

TOAST_TEST(roguelike_shots_are_never_closer_than_the_cooldown)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Input::downKeys[TOAST_KEY_RIGHT] = true;

	// uneven frames: 0.30 s then 0.20 s must not give a 0.20 s gap
	const float frames[] = {0.01f, 0.30f, 0.20f, 0.04f, 0.10f, 0.15f};
	float now = 0;
	float lastShot = -1;
	int shots = 0;
	for(size_t i = 0; i < sizeof(frames) / sizeof(frames[0]); i++)
	{
		TF::elapsed = frames[i];
		now += frames[i];
		world->update();
		if(world->player->shots != shots)
		{
			if(lastShot >= 0)
				CHECK(now - lastShot >= Player::FIRE_COOLDOWN - 0.0001f);
			lastShot = now;
			shots = world->player->shots;
		}
	}
	releaseInput();
	CHECK(shots >= 2);
	delete engine;
}

TOAST_TEST(roguelike_shake_trauma_is_capped_and_decays_to_zero)
{
	ScreenShake shake;
	shake.add(0.8f);
	shake.add(0.8f);
	CHECK(shake.trauma == 1.0f);

	ScreenShake small;
	small.add(0.5f);
	for(int i = 0; i < 10; i++)
		small.update(0.1f);
	CHECK(small.trauma == 0.0f);
	float x, y;
	small.offset(x, y);
	CHECK(x == 0.0f && y == 0.0f);

	ScreenShake strong;
	strong.add(0.5f);
	for(int i = 0; i < 50; i++)
	{
		strong.offset(x, y);
		CHECK(SDL_fabsf(x) <= ScreenShake::MAX_OFFSET * 0.25f && SDL_fabsf(y) <= ScreenShake::MAX_OFFSET * 0.25f);
	}
}

namespace
{
	Cat* addCat(GameWorld* world, Cat::Kind kind, float dx, float dy)
	{
		Room* room = world->room;
		Cat* cat = new Cat(room->x + Room::WIDTH / 2 + dx, room->y + Room::HEIGHT / 2 + dy, kind, room,
						   world->player, world->effects, &world->shake);
		world->cats.push_back(cat);
		world->add(cat);
		return cat;
	}

	void runFor(GameWorld* world, float seconds)
	{
		TF::elapsed = 0.02f;
		for(float t = 0; t < seconds - 0.001f; t += TF::elapsed)
			world->update();
	}
}

TOAST_TEST(roguelike_spawning_cat_is_harmless_and_bullets_pass_over_it)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	world->player->active = false;
	Cat* cat = addCat(world, Cat::ZOOMIE, 0, 0);

	runFor(world, 0.2f);
	CHECK(!cat->active());
	Bullet* through = new Bullet(cat->x - 30, cat->y - 6, 1, 0, world->room, world->effects, &world->cats);
	world->add(through);
	runFor(world, 0.3f);
	CHECK(bulletsIn(world).size() == 1);
	CHECK(through->x > cat->x + 10);
	CHECK(!cat->active());

	float traumaBefore = world->shake.trauma;
	runFor(world, 0.5f);
	CHECK(cat->active());
	CHECK(world->shake.trauma > traumaBefore);

	// the kid on the cat's spot keeps the zoomie in the bullet's path
	world->player->x = cat->x;
	world->player->y = cat->y;
	Bullet* hit = new Bullet(cat->x - 30, cat->y - 6, 1, 0, world->room, world->effects, &world->cats);
	world->add(hit);
	runFor(world, 0.3f);
	CHECK(std::find(world->entities.begin(), world->entities.end(), (Entity*)hit) == world->entities.end());
	delete engine;
}

TOAST_TEST(roguelike_pouncer_crouches_before_it_leaps)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	world->player->active = false;
	Cat* cat = addCat(world, Cat::POUNCER, -60, 0);
	runFor(world, Cat::SPAWN_SECONDS + 0.02f);
	CHECK(cat->active());

	TF::elapsed = 0.02f;
	bool crouched = false;
	bool leapt = false;
	for(int i = 0; i < 100 && !leapt; i++)
	{
		world->update();
		crouched = crouched || cat->pounce == Cat::CROUCHING;
		leapt = cat->pounce == Cat::LEAPING;
	}
	CHECK(crouched && leapt);
	CHECK(cat->lastCrouchSeconds >= Cat::CROUCH_SECONDS);
	CHECK(cat->lastCrouchSeconds < Cat::CROUCH_SECONDS + 0.05f);
	delete engine;
}

TOAST_TEST(roguelike_zoomie_closes_in_on_the_kid)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	world->player->active = false;
	Cat* cat = addCat(world, Cat::ZOOMIE, 80, -30);
	runFor(world, Cat::SPAWN_SECONDS + 0.02f);
	float before = SDL_sqrtf((cat->x - world->player->x) * (cat->x - world->player->x) + (cat->y - world->player->y) * (cat->y - world->player->y));
	runFor(world, 1.0f);
	float after = SDL_sqrtf((cat->x - world->player->x) * (cat->x - world->player->x) + (cat->y - world->player->y) * (cat->y - world->player->y));
	CHECK(after < before - 20);
	delete engine;
}

namespace
{
	GameWorld* startEastGame(Engine*& engine)
	{
		GameWorld* world = startGame(engine);
		unsigned int seed = 1;
		world->newGame(seed);
		while(!world->level.maze.hasDoor(world->level.maze.start, RIGHT) ||
			  world->level.maze.neighbour(world->level.maze.start, RIGHT) == world->level.maze.locked)
			world->newGame(++seed);
		return world;
	}

	void walkThroughDoor(GameWorld* world, Direction d)
	{
		Room* room = world->room;
		int column, row;
		Room::doorTile(d, column, row);
		bool horizontal = d == UP || d == DOWN;
		world->player->x = room->x + column * Room::TILE + (horizontal ? Room::TILE : 2);
		world->player->y = room->y + row * Room::TILE + (horizontal ? 4 : Room::TILE + 4);
		TF::elapsed = 0.05f;
		world->update();
		for(int i = 0; i < 10 && world->sliding(); i++)
			world->update();
	}
}

TOAST_TEST(roguelike_cat_count_grows_with_distance)
{
	CHECK(GameWorld::catCount(1, 6, 6) == GameWorld::MIN_CATS);
	CHECK(GameWorld::catCount(6, 6, 6) == GameWorld::MAX_CATS);
	int previous = 0;
	for(int d = 1; d <= 6; d++)
	{
		int count = GameWorld::catCount(d, 6, 6);
		CHECK(count >= previous);
		CHECK(count >= GameWorld::MIN_CATS && count <= GameWorld::MAX_CATS);
		previous = count;
	}
	CHECK(GameWorld::catCount(6, 6, 3) == 3);
	CHECK(GameWorld::catCount(1, 1, 6) == GameWorld::MIN_CATS);
}

TOAST_TEST(roguelike_entering_a_new_room_closes_it_and_spawns_cats)
{
	Engine* engine;
	GameWorld* world = startEastGame(engine);
	CHECK(world->cats.empty());
	Room* east = world->level.roomAt(world->level.maze.neighbour(world->level.maze.start, RIGHT));
	walkThroughDoor(world, RIGHT);
	CHECK(world->room == east);

	size_t points = 0;
	for(int r = 0; r < RoomTemplates::ROWS; r++)
		for(int c = 0; c < RoomTemplates::COLUMNS; c++)
			points += east->layout->at(c, r) == RoomTemplates::SPAWN;
	CHECK(world->cats.size() >= (size_t)GameWorld::MIN_CATS && world->cats.size() <= (size_t)GameWorld::MAX_CATS);
	CHECK(world->cats.size() <= points);
	for(size_t i = 0; i < world->cats.size(); i++)
	{
		CHECK(!world->cats[i]->active());
		// their cracks show where they will come out during the slide in already
		CHECK(world->cats[i]->graphicsList[1]->point->x == world->cats[i]->x);
	}

	CHECK(world->roomState(east) == GameWorld::FIGHTING);
	int eastCell = world->level.maze.neighbour(world->level.maze.start, RIGHT);
	for(int d = UP; d <= RIGHT; d++)
	{
		if(!world->level.maze.hasDoor(eastCell, (Direction)d))
			continue;
		CHECK(east->door((Direction)d) == DOOR_CLOSED || east->door((Direction)d) == DOOR_LOCKED);
		CHECK(solidDoorTiles(*east, (Direction)d) == 2);
	}
	delete engine;
}

TOAST_TEST(roguelike_room_opens_when_its_last_cat_dies_and_stays_clear)
{
	Engine* engine;
	GameWorld* world = startEastGame(engine);
	Room* start = world->room;
	Room* east = world->level.roomAt(world->level.maze.neighbour(world->level.maze.start, RIGHT));
	walkThroughDoor(world, RIGHT);
	runFor(world, Cat::SPAWN_SECONDS + 0.05f);
	world->player->health = 100;

	std::vector<Cat*> cats = world->cats;
	for(size_t i = 0; i < cats.size(); i++)
		for(int hit = 0; hit < Cat::HIT_POINTS; hit++)
			cats[i]->hit(1, 0);
	int particles = world->effects->particleCount();
	runFor(world, 0.1f);

	CHECK(world->cats.empty());
	CHECK(world->roomState(east) == GameWorld::CLEARED);
	CHECK(east->door(LEFT) == DOOR_OPEN);
	CHECK(solidDoorTiles(*east, LEFT) == 0);
	// dust at the left door, away from where the cats died
	bool dustAtDoor = false;
	for(int i = particles; i < world->effects->particleCount(); i++)
	{
		float px, py;
		world->effects->particlePosition(i, px, py);
		dustAtDoor = dustAtDoor || (SDL_fabsf(px - (east->x + Room::TILE / 2)) < Room::TILE * 2 &&
									SDL_fabsf(py - (east->y + Room::HEIGHT / 2)) < Room::TILE * 2);
	}
	CHECK(dustAtDoor);

	walkThroughDoor(world, LEFT);
	CHECK(world->room == start);
	CHECK(world->cats.empty());
	CHECK(start->door(RIGHT) == DOOR_OPEN);
	walkThroughDoor(world, RIGHT);
	CHECK(world->room == east);
	CHECK(world->cats.empty());
	delete engine;
}

TOAST_TEST(roguelike_locked_room_spawns_nothing)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	const Maze& maze = world->level.maze;
	CHECK(world->roomState(world->level.startRoom()) == GameWorld::CLEARED);
	CHECK(world->roomState(world->level.roomAt(maze.locked)) == GameWorld::CLEARED);

	Direction inward = UP;
	for(int d = UP; d <= RIGHT; d++)
		if(maze.hasDoor(maze.locked, (Direction)d))
			inward = opposite((Direction)d);
	Room* outside = world->level.roomAt(maze.neighbour(maze.locked, opposite(inward)));
	Room* locked = world->level.roomAt(maze.locked);
	world->room = outside;
	world->player->room = outside;
	outside->setDoor(inward, DOOR_OPEN);
	walkThroughDoor(world, inward);

	CHECK(world->room == locked);
	CHECK(world->cats.empty());
	CHECK(locked->door(opposite(inward)) == DOOR_OPEN);
	delete engine;
}

namespace
{
	// keeps the kid standing on the cat for a while, whatever knocks him back
	void standOn(GameWorld* world, Cat* cat, float seconds)
	{
		TF::elapsed = 0.02f;
		for(float t = 0; t < seconds - 0.001f; t += TF::elapsed)
		{
			world->player->x = cat->x;
			world->player->y = cat->y;
			world->update();
		}
	}
}

TOAST_TEST(roguelike_kid_is_hurt_only_by_active_cats)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Cat* cat = addCat(world, Cat::ZOOMIE, 0, 0);

	standOn(world, cat, Cat::SPAWN_SECONDS - 0.1f);
	CHECK(world->player->health == Player::MAX_HEALTH);
	standOn(world, cat, 0.2f);
	CHECK(world->player->health == Player::MAX_HEALTH - 1);
	delete engine;
}

TOAST_TEST(roguelike_two_touches_within_a_second_cost_half_a_heart_once)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Cat* cat = addCat(world, Cat::ZOOMIE, 0, 0);
	runFor(world, Cat::SPAWN_SECONDS + 0.02f);

	standOn(world, cat, 0.9f);
	CHECK(world->player->health == Player::MAX_HEALTH - 1);
	standOn(world, cat, 0.3f);
	CHECK(world->player->health == Player::MAX_HEALTH - 2);
	delete engine;
}

TOAST_TEST(roguelike_last_half_heart_ends_the_game_and_r_restarts)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	Cat* cat = addCat(world, Cat::ZOOMIE, 0, 0);
	runFor(world, Cat::SPAWN_SECONDS + 0.02f);

	world->player->health = 1;
	standOn(world, cat, 0.04f);
	CHECK(world->gameOver());
	CHECK(world->graphicsList.size() > 0);
	int shownTexts = 0;
	for(size_t i = 0; i < world->graphicsList.size(); i++)
		shownTexts += dynamic_cast<Text*>(world->graphicsList[i]) && world->graphicsList[i]->visible;
	CHECK(shownTexts == 2);

	float catX = cat->x;
	runFor(world, 0.5f);
	CHECK(cat->x == catX);

	std::vector<unsigned char> oldDoors = world->level.maze.doors;
	std::vector<int> oldRooms = world->level.maze.rooms;
	Input::pressedKeys[TOAST_KEY_R] = true;
	world->update();
	Input::pressedKeys[TOAST_KEY_R] = false;
	CHECK(world->level.maze.doors != oldDoors || world->level.maze.rooms != oldRooms);
	CHECK(!world->gameOver());
	for(size_t i = 0; i < world->graphicsList.size(); i++)
		CHECK(!dynamic_cast<Text*>(world->graphicsList[i]) || !world->graphicsList[i]->visible);
	CHECK(world->player->health == Player::MAX_HEALTH);
	CHECK(world->cats.empty());
	CHECK(world->room == world->level.startRoom());
	delete engine;
}

TOAST_TEST(roguelike_cat_dies_on_its_third_hit_in_a_burst_of_fur)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	world->player->active = false;
	world->player->x = world->room->x + Room::WIDTH / 2;
	world->player->y = world->room->y + Room::HEIGHT / 2;
	// a pouncer right next to the kid crouches in place, so it stays in the line of fire
	Cat* cat = addCat(world, Cat::POUNCER, 40, 0);
	runFor(world, Cat::SPAWN_SECONDS + 0.02f);

	for(int shot = 1; shot <= 3; shot++)
	{
		int particles = world->effects->particleCount();
		float trauma = world->shake.trauma;
		world->add(new Bullet(cat->x - 20, cat->y - 6, 1, 0, world->room, world->effects, &world->cats));
		TF::elapsed = 0.02f;
		for(int i = 0; i < 15 && !bulletsIn(world).empty(); i++)
			world->update();
		if(shot < 3)
		{
			CHECK(world->cats.size() == 1);
			CHECK(cat->hitPoints == Cat::HIT_POINTS - shot);
		}
		else
		{
			CHECK(world->cats.empty());
			CHECK(std::find(world->entities.begin(), world->entities.end(), (Entity*)cat) == world->entities.end());
			CHECK(world->effects->particleCount() >= particles + 10);
			CHECK(world->shake.trauma > trauma);
		}
	}
	delete engine;
}

TOAST_TEST(roguelike_cat_finds_its_way_around_a_wall)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	world->player->active = false;

	RoomTemplate walled = layoutWith(RoomTemplates::FLOOR, 0, 0);
	walled.rows[4] = "..##############..";
	Room room(walled, 0, 20, 20);
	float middleX = room.x + Room::WIDTH / 2;
	world->player->x = middleX;
	world->player->y = room.y + 9 * Room::TILE;

	for(int k = 0; k < 2; k++)
	{
		Cat* cat = new Cat(middleX, room.y + 3 * Room::TILE, k == 0 ? Cat::ZOOMIE : Cat::POUNCER, &room,
						   world->player, world->effects, &world->shake);
		world->cats.push_back(cat);
		world->add(cat);
	}
	// the pouncer stalks at 30 px/s, and the way round is about 200 px
	runFor(world, 10.0f);
	for(size_t i = 0; i < world->cats.size(); i++)
		CHECK(world->cats[i]->y > room.y + 6 * Room::TILE);
	world->cats.clear();
	world->removeAll();
	delete engine;
}

namespace
{
	void killAll(GameWorld* world)
	{
		runFor(world, Cat::SPAWN_SECONDS + 0.05f);
		std::vector<Cat*> cats = world->cats;
		for(size_t i = 0; i < cats.size(); i++)
			for(int hit = 0; hit < Cat::HIT_POINTS; hit++)
				cats[i]->hit(1, 0);
		runFor(world, 0.1f);
	}
}

TOAST_TEST(roguelike_level_counts_the_cats_of_every_room_before_they_spawn)
{
	Engine* engine;
	GameWorld* world = startGame(engine);
	int fightingRooms = (int)world->level.maze.rooms.size() - 2;
	CHECK(world->catsLeft >= GameWorld::MIN_CATS * fightingRooms);
	CHECK(world->catsLeft <= GameWorld::MAX_CATS * fightingRooms);
	CHECK(world->key == NULL);
	delete engine;
}

TOAST_TEST(roguelike_clearing_a_room_while_others_have_cats_gives_no_key)
{
	Engine* engine;
	GameWorld* world = startEastGame(engine);
	int before = world->catsLeft;
	walkThroughDoor(world, RIGHT);
	world->player->health = 100;
	int spawned = (int)world->cats.size();
	killAll(world);
	CHECK(world->catsLeft == before - spawned);
	CHECK(world->catsLeft > 0);
	CHECK(world->key == NULL);
	delete engine;
}

TOAST_TEST(roguelike_last_cat_of_the_level_drops_the_key)
{
	Engine* engine;
	GameWorld* world = startEastGame(engine);
	walkThroughDoor(world, RIGHT);
	world->player->health = 100;
	world->catsLeft = (int)world->cats.size();
	int particles = world->effects->particleCount();
	runFor(world, Cat::SPAWN_SECONDS + 0.05f);
	std::vector<Cat*> cats = world->cats;
	for(size_t i = 0; i < cats.size(); i++)
		for(int hit = 0; hit < Cat::HIT_POINTS; hit++)
			cats[i]->hit(1, 0);
	TF::elapsed = 0.02f;
	world->update();

	CHECK(world->catsLeft == 0);
	CHECK(world->key != NULL);
	// drawn where it lies from its first frame, while the kill freeze stops updates
	CHECK(world->freezeTime > 0);
	CHECK(world->key->graphic->point->x == world->key->x && world->key->graphic->point->y == world->key->y);
	CHECK(world->key->x == world->room->x + Room::WIDTH / 2);
	CHECK(world->key->y == world->room->y + Room::HEIGHT / 2);
	CHECK(world->effects->particleCount() > particles);
	CHECK(!world->hasKey);

	world->player->x = world->key->x;
	world->player->y = world->key->y;
	runFor(world, 0.1f);
	CHECK(world->hasKey);
	CHECK(world->key == NULL);
	delete engine;
}
