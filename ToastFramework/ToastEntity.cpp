/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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