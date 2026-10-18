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
#include "ToastFont.h"
#include "ToastText.h"
#include "ToastEngine.h"
#include "ToastInput.h"
#include "ToastRandom.h"
#include <cmath>

namespace Toast
{
	const float GameWorld::SLIDE_SECONDS = 0.3f;
	const float GameWorld::KILL_FREEZE_SECONDS = 0.04f;

	namespace
	{
		const float KILL_SHAKE = 0.35f;
		const float HURT_SHAKE = 0.45f;
		const int FUR_TUFTS = 10;
	}

	GameWorld::GameWorld() :
		World(),
		player(NULL),
		room(NULL),
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
		freezeTime = 0;
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
				continue;
			}

			bool touching = player->collideRect(player->x, player->y, cat->x - cat->originX, cat->y - cat->originY,
												(float)cat->width, (float)cat->height);
			if(cat->active() && touching && player->hurt(cat->x, cat->y))
				shake.add(HURT_SHAKE);
			i++;
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
		gameOverText->visible = gameOver();
		restartText->visible = gameOver();
		if(gameOver())
			return;

		int door = doorUnderPlayer();
		if(door >= 0)
			enterNextRoom((Direction)door);
	}
}
