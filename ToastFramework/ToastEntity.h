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

#ifndef TOASTENTITY_H
#define TOASTENTITY_H

#include "ToastCommon.h"

namespace Toast
{
	class Entity
	{
	public:
		Entity(float x = 0.0f, float y = 0.0f);
		virtual ~Entity();

		void setHitbox(int width, int height, float originX, float originY);
		bool collideRect(float x, float y, float rX, float rY, float rWidth, float rHeight);

		virtual void update();
		void render();

		/**
		* Rendering stuff
		*/
		Graphic* graphic;
		//
		
		bool active;
		bool visible;
		bool collidable;
		float x;
		float y;
		float alpha;

		/**
		* Width and height of the Entity's hitbox
		*/
		int width;
		int height;

		/**
		* Coordinates of the Entity's hitbox
		*/
		float originX;
		float originY;

		/**
		* Collision temp variables
		*/
		float _x;
		float _y;
	};
}

#endif // TOASTENTITY_H