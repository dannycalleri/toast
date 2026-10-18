/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "ToastCommon.h"
#include "ToastWorld.h"
#include "Level.h"
#include "Player.h"
#include "ScreenShake.h"
#include <string>
#include <vector>

namespace Toast
{
	class Cat;
	class Emitter;
	class Player;
	class Room;

	class GameWorld : public World
	{
	public:
		static const int HUD_HEIGHT = 48;
		static const float SLIDE_SECONDS;
		static const float KILL_FREEZE_SECONDS;
		static const int MIN_CATS = 2;
		static const int MAX_CATS = 5;

		enum RoomState { UNVISITED, FIGHTING, CLEARED };

		// MIN_CATS one door from the start, MAX_CATS at the farthest room,
		// never more than the room has spawn points
		static int catCount(int distance, int maxDistance, int spawnPoints);

		GameWorld();
		virtual ~GameWorld();

		virtual void update();
		virtual void render();

		void newGame(unsigned int seed);
		bool sliding() const { return slideTime >= 0; }
		bool gameOver() const { return player && !player->alive(); }
		RoomState roomState(const Room* room) const;

		Level level;
		Player* player;
		Room* room;
		Emitter* effects;
		ScreenShake shake;
		std::vector<Cat*> cats;
		float freezeTime;

	private:
		void showRoom(Room* room);
		void enterNextRoom(Direction d);
		// the door of the current room the player stands in, or -1
		int doorUnderPlayer();
		int cellOf(const Room* room) const;
		void fight(Room* room);
		void clear(Room* room);
		// removes dead cats and lets the live ones hurt the kid
		void settleCats();
		Text* centredText(const std::string& line, Font* font, float y);

		std::vector<RoomState> roomStates;

		Canvas* hudBand;
		Font* bigFont;
		Font* smallFont;
		Text* gameOverText;
		Text* restartText;
		float slideTime;
		float slideFromX;
		float slideFromY;
	};
}

#endif // GAMEWORLD_H
