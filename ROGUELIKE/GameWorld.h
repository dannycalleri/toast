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

namespace Toast
{
	class Emitter;
	class Player;
	class Room;

	class GameWorld : public World
	{
	public:
		static const int HUD_HEIGHT = 48;
		static const float SLIDE_SECONDS;

		GameWorld();
		virtual ~GameWorld();

		virtual void update();

		void newGame(unsigned int seed);
		bool sliding() const { return slideTime >= 0; }

		Level level;
		Player* player;
		Room* room;
		Emitter* effects;

	private:
		void showRoom(Room* room);
		void enterNextRoom(Direction d);
		// the door of the current room the player stands in, or -1
		int doorUnderPlayer();

		Canvas* hudBand;
		float slideTime;
		float slideFromX;
		float slideFromY;
	};
}

#endif // GAMEWORLD_H
