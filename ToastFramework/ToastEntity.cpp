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

#include "ToastEntity.h"
#include "ToastGraphic.h"

#include <iostream>

namespace Toast
{
	Entity::Entity(float x, float y)
	{
		this->graphic = 0;
		this->x = x;
		this->y = y;
		active = true;
		visible = true;
		collidable = true;
		width = 0;
		height = 0;
		originX=0;
		originY=0;
		_x = 0;
		_y = 0;
		alpha=1.0f;
	}

	Entity::~Entity()
	{
	}

	void Entity::setHitbox(int width, int height, float originX, float originY)
	{
		this->width = width;
		this->height = height;
		this->originX = originX;
		this->originY = originY;
	}

	bool Entity::collideRect(float x, float y, float rX, float rY, float rWidth, float rHeight)
	{
		if (x - originX + width >= rX && y - originY + height >= rY &&
			x - originX <= rX + rWidth && y - originY <= rY + rHeight)
		{
			return true;
		}

		return false;
	}

	void Entity::update()
	{
		if(graphic && graphic->active)
		{
			graphic->x = this->x;
			graphic->y = this->y;
			graphic->alpha = this->alpha;
			graphic->update();
		}
	}

	void Entity::render()
	{
		if(graphic && graphic->visible)
		{
			//std::cout << "ENTITY DRAW\n";
			graphic->render();

			//if(TF::debug)
			//{
			//	//std::cout << "ENEMENE\n";
			//	al_draw_rectangle(this->x, this->y, this->x + this->width, this->y + this->height, al_map_rgb(255,0,0), 2);
			//}
		}
	}
}