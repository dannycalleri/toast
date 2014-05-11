#ifndef LEVEL_H
#define LEVEL_H

#include "ToastCommon.h"
#include "ToastEntity.h"
#include "ToastTilemap.h"

namespace Toast
{
	class Level : public Entity
	{
	public:
		Level()
		{
			tileMap = new Tilemap("tile_caverna.png", 512*10, 400*8, 512, 400);
			tileMap->setTile(0,0,1);
			tileMap->setTile(0,1,1);


			std::string level = "1,1,1,1,1,1,1,1,1,1\n"
								"0,0,0,0,0,0,0,0,0,0\n"
								"1,1,1,1,1,1,1,1,1,1\n"
								"0,0,0,0,0,0,0,0,0,0\n"
								"0,0,0,0,0,0,0,0,0,0\n"
								"1,1,1,1,1,1,1,1,1,1\n"
								"1,1,1,1,1,1,1,1,1,1\n"
								"0,0,0,0,0,0,0,0,0,0\n";

			tileMap->loadFromString(level);
			tileMap->alpha=1.0f;

			graphic = tileMap;
		}

		~Level() {}

		//virtual void update() { Entity::update(); }

	private:
		Tilemap* tileMap;
	};
}

#endif