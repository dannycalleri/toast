/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "GameWorld.h"
#include "Effects.h"
#include "Player.h"
#include "Room.h"
#include "TF.h"
#include "ToastCanvas.h"
#include "ToastEmitter.h"
#include "ToastEngine.h"
#include "ToastInput.h"
#include "ToastRandom.h"

namespace Toast
{
	const float GameWorld::SLIDE_SECONDS = 0.3f;

	GameWorld::GameWorld() :
		World(),
		player(NULL),
		room(NULL),
		slideTime(-1)
	{
		// added before the HUD band, so particles never draw over the HUD
		effects = Effects::create();
		addGraphic(effects);

		hudBand = new Canvas(Room::WIDTH, HUD_HEIGHT, false);
		hudBand->clear(0x1a, 0x1c, 0x2c);
		hudBand->scrollX = 0;
		hudBand->scrollY = 0;
		addGraphic(hudBand);

		if(level.loadTemplates())
			newGame(TF::engine->random->Random(0x7fffffff));
	}

	GameWorld::~GameWorld()
	{
	}

	void GameWorld::newGame(unsigned int seed)
	{
		// everything in the world belongs to the old maze; bullets point at its rooms
		removeAll();
		level.forgetRooms();
		player = NULL;
		room = NULL;
		slideTime = -1;

		if(!level.build(this, seed))
			return;

		room = level.startRoom();
		player = new Player(room->x + Room::WIDTH / 2, room->y + Room::HEIGHT / 2 + Room::TILE * 2);
		player->room = room;
		player->effects = effects;
		add(player);
		showRoom(room);
	}

	void GameWorld::showRoom(Room* shown)
	{
		TF::setCamera(shown->x, shown->y - HUD_HEIGHT);
	}

	int GameWorld::doorUnderPlayer()
	{
		for(int d = UP; d <= RIGHT; d++)
		{
			if(room->door((Direction)d) != DOOR_OPEN)
				continue;
			int column, row;
			Room::doorTile((Direction)d, column, row);
			bool horizontal = d == UP || d == DOWN;
			float doorX = room->x + column * Room::TILE;
			float doorY = room->y + row * Room::TILE;
			float doorWidth = Room::TILE * (horizontal ? 2 : 1);
			float doorHeight = Room::TILE * (horizontal ? 1 : 2);
			// strict overlap: walking along the wall past a door touches it without entering
			float left = player->x - player->originX;
			float top = player->y - player->originY;
			if(left < doorX + doorWidth && left + player->width > doorX &&
			   top < doorY + doorHeight && top + player->height > doorY)
				return d;
		}
		return -1;
	}

	void GameWorld::enterNextRoom(Direction d)
	{
		Room* next = level.roomAt(level.maze.neighbour(room->row * level.maze.width + room->column, d));
		if(!next)
			return;

		slideFromX = TF::camera->x;
		slideFromY = TF::camera->y;
		slideTime = 0;
		room = next;
		player->room = next;
		player->active = false;

		// just inside the door the kid came through, clear of its tiles
		float middleX = next->x + Room::WIDTH / 2;
		float middleY = next->y + Room::HEIGHT / 2 + player->height;
		switch(d)
		{
		case UP:    player->x = middleX; player->y = next->y + (Room::ROWS - 2) * Room::TILE - 1; break;
		case DOWN:  player->x = middleX; player->y = next->y + 2 * Room::TILE + player->originY; break;
		case LEFT:  player->x = next->x + (Room::COLUMNS - 2) * Room::TILE - player->originX - 1; player->y = middleY; break;
		case RIGHT: player->x = next->x + 2 * Room::TILE + player->originX; player->y = middleY; break;
		}
		player->Entity::update();
	}

	void GameWorld::update()
	{
		if(Input::pressed(TOAST_KEY_ESCAPE))
			TF::engine->onExit();

		if(Input::pressed(TOAST_KEY_R))
			newGame(TF::engine->random->Random(0x7fffffff));

		if(!room)
			return;

		if(sliding())
		{
			slideTime += TF::elapsed;
			float t = slideTime >= SLIDE_SECONDS ? 1.0f : slideTime / SLIDE_SECONDS;
			float toX = room->x;
			float toY = room->y - HUD_HEIGHT;
			TF::setCamera(slideFromX + (toX - slideFromX) * t, slideFromY + (toY - slideFromY) * t);
			if(t >= 1.0f)
			{
				slideTime = -1;
				player->active = true;
			}
			return;
		}

		World::update();

		int door = doorUnderPlayer();
		if(door >= 0)
			enterNextRoom((Direction)door);
	}
}
