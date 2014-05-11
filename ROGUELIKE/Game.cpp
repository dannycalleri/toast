/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Game.h"
#include "GameWorld.h"

namespace Toast
{
	const int Game::TILESIZE = 32;

	Game::Game()
		: Engine(1024, 768, false)
	{
		// giving the game a world to update
		TF::world = new GameWorld();
		TF::debug = true;

		TF::console->enable();
		// debug console settings
		TF::console->setThickness(5);
	}

}