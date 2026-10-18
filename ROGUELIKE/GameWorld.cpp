/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "GameWorld.h"
#include "Cat.h"
#include "Effects.h"
#include "Player.h"
#include "Room.h"
#include "RoomTemplates.h"
#include "TF.h"
#include "ToastCanvas.h"
#include "ToastEmitter.h"
#include "ToastEngine.h"
#include "ToastInput.h"
#include "ToastRandom.h"
#include <cmath>

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
		cats.clear();
		player = NULL;
		room = NULL;
		slideTime = -1;

		if(!level.build(this, seed))
			return;

		visited.assign(level.maze.width * level.maze.height, false);
		visited[level.maze.start] = true;
		room = level.startRoom();
		player = new Player(room->x + Room::WIDTH / 2, room->y + Room::HEIGHT / 2 + Room::TILE * 2);
		player->room = room;
		player->effects = effects;
		player->cats = &cats;
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

		int cell = next->row * level.maze.width + next->column;
		if(!visited[cell])
		{
			visited[cell] = true;
			spawnCats(next);
		}
	}

	void GameWorld::spawnCats(Room* where)
	{
		int cell = where->row * level.maze.width + where->column;
		if(cell == level.maze.locked)
			return;

		for(int r = 0; r < RoomTemplates::ROWS; r++)
		{
			for(int c = 0; c < RoomTemplates::COLUMNS; c++)
			{
				if(where->layout->at(c, r) != RoomTemplates::SPAWN)
					continue;
				float x = where->x + (c + 1) * Room::TILE + Room::TILE / 2;
				float y = where->y + (r + 1) * Room::TILE + Room::TILE - 4;
				Cat::Kind kind = TF::random() < 0.5f ? Cat::ZOOMIE : Cat::POUNCER;
				Cat* cat = new Cat(x, y, kind, where, player, effects, &shake);
				cats.push_back(cat);
				add(cat);
			}
		}
	}

	void GameWorld::render()
	{
		float shakeX, shakeY;
		shake.offset(shakeX, shakeY);
		// whole pixels, so the pixel art stays sharp while it shakes
		shakeX = std::round(shakeX);
		shakeY = std::round(shakeY);
		TF::camera->x += shakeX;
		TF::camera->y += shakeY;
		World::render();
		TF::camera->x -= shakeX;
		TF::camera->y -= shakeY;
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

		shake.update(TF::elapsed);
		World::update();

		int door = doorUnderPlayer();
		if(door >= 0)
			enterNextRoom((Direction)door);
	}
}
