/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "GameWorld.h"
#include "Cat.h"
#include "Effects.h"
#include "Key.h"
#include "Player.h"
#include "Room.h"
#include "RoomTemplates.h"
#include "TF.h"
#include "ToastCanvas.h"
#include "ToastEmitter.h"
#include "ToastFont.h"
#include "ToastText.h"
#include "ToastEngine.h"
#include "ToastInput.h"
#include "ToastRandom.h"
#include <cmath>
#include <algorithm>
#include <utility>

namespace Toast
{
	const float GameWorld::SLIDE_SECONDS = 0.3f;
	const float GameWorld::KILL_FREEZE_SECONDS = 0.04f;

	namespace
	{
		const float KILL_SHAKE = 0.35f;
		const float HURT_SHAKE = 0.45f;
		const int FUR_TUFTS = 10;
		const int DOOR_DUST = 6;
		const int KEY_SPARKLES = 16;
	}

	GameWorld::GameWorld() :
		World(),
		player(NULL),
		room(NULL),
		key(NULL),
		hasKey(false),
		catsLeft(0),
		freezeTime(0.0f),
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

		bigFont = new Font("assets/pixelfont.TTF", 24);
		smallFont = new Font("assets/pixelfont.TTF", 8);
		gameOverText = centredText("GAME OVER", bigFont, 100);
		restartText = centredText("PRESS R TO TRY A NEW CRYPT", smallFont, 136);

		if(level.loadTemplates())
			newGame(TF::engine->random->Random(0x7fffffff));
	}

	GameWorld::~GameWorld()
	{
		delete bigFont;
		delete smallFont;
	}

	Text* GameWorld::centredText(const std::string& line, Font* font, float y)
	{
		Text* text = new Text(line);
		text->font = font;
		text->updateCanvas();
		text->x = (TF::width - text->width) / 2.0f;
		text->y = y;
		text->scrollX = 0;
		text->scrollY = 0;
		text->visible = false;
		addGraphic(text);
		return text;
	}

	void GameWorld::newGame(unsigned int seed)
	{
		// everything in the world belongs to the old maze; bullets point at its rooms
		removeAll();
		level.forgetRooms();
		cats.clear();
		key = NULL;
		hasKey = false;
		freezeTime = 0;
		player = NULL;
		room = NULL;
		slideTime = -1;

		if(!level.build(this, seed))
			return;

		roomStates.assign(level.maze.width * level.maze.height, UNVISITED);
		roomStates[level.maze.start] = CLEARED;
		roomStates[level.maze.locked] = CLEARED;

		int maxDistance = 0;
		for(size_t i = 0; i < level.maze.rooms.size(); i++)
			maxDistance = std::max(maxDistance, level.maze.distance[level.maze.rooms[i]]);
		roomCats.assign(level.maze.width * level.maze.height, 0);
		catsLeft = 0;
		for(size_t i = 0; i < level.maze.rooms.size(); i++)
		{
			int cell = level.maze.rooms[i];
			if(roomStates[cell] == CLEARED)
				continue;
			roomCats[cell] = catCount(level.maze.distance[cell], maxDistance, spawnPoints(level.roomAt(cell)));
			catsLeft += roomCats[cell];
		}
		room = level.startRoom();
		player = new Player(room->x + Room::WIDTH / 2, room->y + Room::HEIGHT / 2 + Room::TILE * 2);
		player->room = room;
		player->effects = effects;
		player->cats = &cats;
		add(player);
		showRoom(room);
		if(catsLeft == 0)
			dropKey(room->x + Room::WIDTH / 2, room->y + Room::HEIGHT / 2);
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

		if(roomStates[cellOf(next)] == UNVISITED)
			fight(next);
	}

	int GameWorld::catCount(int distance, int maxDistance, int spawnPoints)
	{
		int count = MIN_CATS;
		if(maxDistance > 1 && distance > 1)
			count += (int)std::round((float)(MAX_CATS - MIN_CATS) * (distance - 1) / (maxDistance - 1));
		if(count > MAX_CATS)
			count = MAX_CATS;
		return count < spawnPoints ? count : spawnPoints;
	}

	int GameWorld::spawnPoints(const Room* where) const
	{
		int points = 0;
		for(int r = 0; r < RoomTemplates::ROWS; r++)
			for(int c = 0; c < RoomTemplates::COLUMNS; c++)
				points += where->layout->at(c, r) == RoomTemplates::SPAWN;
		return points;
	}

	int GameWorld::cellOf(const Room* where) const
	{
		return where->row * level.maze.width + where->column;
	}

	GameWorld::RoomState GameWorld::roomState(const Room* where) const
	{
		return roomStates[cellOf(where)];
	}

	void GameWorld::fight(Room* where)
	{
		std::vector<int> points;
		for(int r = 0; r < RoomTemplates::ROWS; r++)
		{
			for(int c = 0; c < RoomTemplates::COLUMNS; c++)
			{
				if(where->layout->at(c, r) == RoomTemplates::SPAWN)
					points.push_back(r * RoomTemplates::COLUMNS + c);
			}
		}

		int count = roomCats[cellOf(where)];
		if(count == 0)
		{
			roomStates[cellOf(where)] = CLEARED;
			return;
		}

		roomStates[cellOf(where)] = FIGHTING;
		for(int d = UP; d <= RIGHT; d++)
		{
			if(where->door((Direction)d) == DOOR_OPEN)
				where->setDoor((Direction)d, DOOR_CLOSED);
		}

		for(int i = 0; i < count; i++)
		{
			int remaining = (int)points.size() - i;
			int pick = i + std::min((int)(TF::random() * remaining), remaining - 1);
			std::swap(points[i], points[pick]);
			int c = points[i] % RoomTemplates::COLUMNS;
			int r = points[i] / RoomTemplates::COLUMNS;
			float x = where->x + (c + 1) * Room::TILE + Room::TILE / 2;
			float y = where->y + (r + 1) * Room::TILE + Room::TILE - 4;
			Cat::Kind kind = TF::random() < 0.5f ? Cat::ZOOMIE : Cat::POUNCER;
			Cat* cat = new Cat(x, y, kind, where, player, effects, &shake);
			cats.push_back(cat);
			add(cat);
		}
	}

	void GameWorld::clear(Room* where)
	{
		roomStates[cellOf(where)] = CLEARED;
		for(int d = UP; d <= RIGHT; d++)
		{
			if(where->door((Direction)d) != DOOR_CLOSED)
				continue;
			where->setDoor((Direction)d, DOOR_OPEN);

			int column, row;
			Room::doorTile((Direction)d, column, row);
			bool horizontal = d == UP || d == DOWN;
			float doorX = where->x + (column + (horizontal ? 1 : 0.5f)) * Room::TILE;
			float doorY = where->y + (row + (horizontal ? 0.5f : 1)) * Room::TILE;
			for(int puff = 0; puff < DOOR_DUST; puff++)
				effects->emit(Effects::DIRT_BURST, doorX, doorY);
		}
	}

	void GameWorld::settleCats()
	{
		for(size_t i = 0; i < cats.size(); )
		{
			Cat* cat = cats[i];
			if(cat->dead())
			{
				const char* fur = cat->kind == Cat::ZOOMIE ? Effects::FUR : Effects::GREY_FUR_TUFT;
				for(int tuft = 0; tuft < FUR_TUFTS; tuft++)
					effects->emit(fur, cat->x, cat->y - 6);
				shake.add(KILL_SHAKE);
				freezeTime = KILL_FREEZE_SECONDS;
				cats.erase(cats.begin() + i);
				remove(cat);
				catsLeft--;
				if(catsLeft == 0)
					dropKey(room->x + Room::WIDTH / 2, room->y + Room::HEIGHT / 2);
				continue;
			}

			bool touching = player->collideRect(player->x, player->y, cat->x - cat->originX, cat->y - cat->originY,
												(float)cat->width, (float)cat->height);
			if(cat->active() && touching && player->hurt(cat->x, cat->y))
				shake.add(HURT_SHAKE);
			i++;
		}
	}

	void GameWorld::dropKey(float x, float y)
	{
		key = new Key(x, y);
		add(key);
		for(int i = 0; i < KEY_SPARKLES; i++)
			effects->emit(Effects::SPARKLE, x, y - 4);
	}

	void GameWorld::takeKey()
	{
		for(int i = 0; i < KEY_SPARKLES; i++)
			effects->emit(Effects::SPARKLE, key->x, key->y - 4);
		remove(key);
		key = NULL;
		hasKey = true;
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
		gameOverText->visible = gameOver();
		restartText->visible = gameOver();
		if(gameOver())
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
		if(freezeTime > 0)
		{
			freezeTime -= TF::elapsed;
			return;
		}

		World::update();
		settleCats();
		if(cats.empty() && roomStates[cellOf(room)] == FIGHTING)
			clear(room);
		if(key && player->collideRect(player->x, player->y, key->x - key->originX, key->y - key->originY,
									  (float)key->width, (float)key->height))
			takeKey();
		gameOverText->visible = gameOver();
		restartText->visible = gameOver();
		if(gameOver())
			return;

		int door = doorUnderPlayer();
		if(door >= 0)
			enterNextRoom((Direction)door);
	}
}
