#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "ToastCommon.h"
#include "ToastWorld.h"
#include "Level.h"

#include <string>

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
		
		Spritemap* spriteMap;
		Spritemap* spriteMap2;
		//Tilemap* tileMap;

		Level* level;

		Entity* goblin1;
		Entity* goblin2;
		int dir;

		Text* demoText;
		Text* pressEnter;
		Image* controlsHelp;
		
	};
}

#endif // GAMEWORLD_H