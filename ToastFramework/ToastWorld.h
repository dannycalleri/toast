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
		virtual void render();

		void add(Entity* e);

		// removes Entity from update and rendering
		// lists and destroys memory
		void remove(Entity* e);
		// removes and deletes the entity at the end of this frame's update,
		// so an entity can remove itself (or another) while the world updates
		void removeLater(Entity* e);
		void removeAll();
		void addGraphic(Graphic* graphic);

		// if the engine should update
		// the world
		bool active;
		// if the engine should render
		// the world
		bool visible;


		std::vector<Entity*> entities;
		std::vector<Graphic*> graphicsList;

	private:
		void removeQueued();

		std::vector<Entity*> removeQueue;
	};
}


#endif // TOASTWORLD_H