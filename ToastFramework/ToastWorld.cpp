/*
=========================================================================

Toast 2D game framework
Copyright (C)2012 Danny Calleri

Toast 2D game framework is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

==========================================================================
*/

#include "ToastCommon.h"
#include "ToastWorld.h"
#include "ToastGraphic.h"
#include "ToastEntity.h"

#include <iostream>

namespace Toast
{
	World::World():
		active(true),
		visible(true)
	{
		entities.clear();
		graphicsList.clear();
	}

	World::~World()
	{
		for(std::vector<Entity*>::size_type i = 0; i < entities.size(); i++)
		{
			//std::cout << "UPDATING ENTITY " << i << "\n";

			//std::cout << "[WORLD] UPDATING ENTITY\n";
			delete entities[i];
		}

		for(std::vector<Graphic*>::size_type i = 0; i < graphicsList.size(); i++)
		{
			std::cout << "[WORLD destroy] destroying graphic " << i << "\n";
			delete graphicsList[i];
		}
	}

	void World::add(Entity* e)
	{
		entities.push_back(e);
	}

	void World::addGraphic(Graphic* graphic)
	{
		graphicsList.push_back(graphic);
	}

	void World::update()
	{

		for(std::vector<Entity*>::size_type i = 0; i < entities.size(); i++)
		{
			Entity* e = entities[i];
			if(e->active) e->update();
		}

		for(std::vector<Graphic*>::size_type i = 0; i < graphicsList.size(); i++)
		{
			Graphic* g = graphicsList[i];
			if(g->active) g->update();
		}
	}

	void World::render()
	{

		for(std::vector<Graphic*>::size_type i = 0; i < graphicsList.size(); i++)
		{
			Graphic* g = graphicsList[i];
			if(g->visible) g->render();
		}

		for(std::vector<Entity*>::size_type i = 0; i < entities.size(); i++)
		{
			Entity* e = entities[i];
			if(e->visible) e->render();
		}

	}
}