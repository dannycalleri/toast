/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "ToastCommon.h"
#include "ToastWorld.h"

#include <string>

namespace Toast
{
	class RoomsManager;
	class Dungeon;

	class GameWorld : public World
	{
	public:
		GameWorld();
		virtual ~GameWorld();

		// overriding methods inherited
		// from super class
		virtual void update();

	public:
		
		Font* font;
		RoomsManager* roomsManager;
		Dungeon* dungeon;
		bool generateDungeon;

		Text* demoText;
		Text* pressEnter;
		Text* generatingText;
	};
}

#endif // GAMEWORLD_H