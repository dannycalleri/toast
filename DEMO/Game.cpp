#include "Game.h"
#include "GameWorld.h"

namespace Toast
{
	Game::Game()
		: Engine(2560, 1600, false)
	{
		// giving the game a world to update
		TF::world = new GameWorld();
		TF::debug = true;

		TF::console->enable();
		// debug console settings
		TF::console->setThickness(5);
	}

}