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
		Level level;
		bool generateDungeon;

		Text* demoText;
		Text* pressEnter;
		Text* generatingText;
	};
}

#endif // GAMEWORLD_H