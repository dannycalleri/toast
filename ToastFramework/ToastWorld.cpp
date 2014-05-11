/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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
			TOAST_SAFE_DELETE(entities[i]);
		}

		for(std::vector<Graphic*>::size_type i = 0; i < graphicsList.size(); i++)
		{
			//std::cout << "[WORLD destroy] destroying graphic " << i << "\n";
			TOAST_SAFE_DELETE(graphicsList[i]);
		}
	}

	void World::add(Entity* e)
	{
		entities.push_back(e);
	}

	void World::remove(Entity* e)
	{
		for(std::vector<Entity*>::size_type i = 0; i < entities.size(); i++)
		{
			if(entities[i] == e)
			{
				// destroy Entity's memory
				TOAST_SAFE_DELETE(e);
				// delete pointer from array
				entities.erase(entities.begin() + i);
				break;
			}
		}
	}

	void World::removeAll()
	{
		// Destroy memory for all Entities
		for(std::vector<Entity*>::size_type i = 0; i < entities.size(); i++)
		{
			TOAST_SAFE_DELETE(entities[i]);
		}

		entities.clear();
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
			if(e->active) 
				e->update();
		}

		for(std::vector<Graphic*>::size_type i = 0; i < graphicsList.size(); i++)
		{
			Graphic* g = graphicsList[i];
			if(g->active) g->update();
		}
	}

	void World::render()
	{

		for(std::vector<Entity*>::size_type i = 0; i < entities.size(); i++)
		{
			Entity* e = entities[i];
			if(e->visible) e->render();
		}

		for(std::vector<Graphic*>::size_type i = 0; i < graphicsList.size(); i++)
		{
			Graphic* g = graphicsList[i];
			if(g->visible) g->render();
		}
	}
}