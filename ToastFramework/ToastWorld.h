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