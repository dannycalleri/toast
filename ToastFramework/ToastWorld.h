/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTWORLD_H
#define TOASTWORLD_H

#include "ToastCommon.h"
#include <vector>

namespace Toast
{
	class World
	{
	public:
		World();
		virtual ~World();

		virtual void update();
		void render();

		void add(Entity* e);
		void addGraphic(Graphic* graphic);

		// if the engine should update
		// the world
		bool active;
		// if the engine should render
		// the world
		bool visible;


		std::vector<Entity*> entities;
		std::vector<Graphic*> graphicsList;

	};
}


#endif // TOASTWORLD_H