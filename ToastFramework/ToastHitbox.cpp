/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastHitbox.h"
#include "ToastEntity.h"

#include <iostream>
#include <functional>

namespace Toast
{
	Hitbox::Hitbox(int width = 1, int height = 1, float x = 0.0f, float y = 0.0f):
		Mask()
	{
		this->width = width;
		this->height = height;
		this->x = x;
		this->y = y;
		type = HITBOX;
		/*check[type] = &Hitbox::collideHitbox;
		check[MASK] = reinterpret_cast<CollideFunction>(&Hitbox::collideMask);*/
	}

	Hitbox::~Hitbox()
	{
	}

	bool Hitbox::collide(Mask* mask)
	{
		bool typeFound = false;
		bool isColliding = false;

		switch(mask->type)
		{
		case MASK:
			{
				typeFound=true;
				//return collideMask(mask);
				isColliding = collideMask(mask);

				break;
			}
		case HITBOX:
			{
				typeFound=true;
				//return collideHitbox((Hitbox*)mask);
				isColliding = collideHitbox((Hitbox*)mask);

				break;
			}
		}
		
		if(!typeFound && !isColliding)
		{
			isColliding = mask->collide(this);
		}

		return isColliding;
	}

	bool Hitbox::collideMask(Mask* other)
	{
		std::cout << "[HITBOX] collideMask" << "\n";
		return false;
		/*return parent->x + x + width > other->parent->x - other->parent->originX
				&& parent->y + y + height > other->parent->y - other->parent->originY
				&& parent->x + x < other->parent->x - other->parent->originX + other->parent->width
				&& parent->y + y < other->parent->y - other->parent->originY + other->parent->height;*/
	}
	
	bool Hitbox::collideHitbox(Hitbox* other)
	{
		std::cout << "[HITBOX] collideHitbox" << "\n";
		return false;
		/*return parent->x + x + width > other->parent->x + other->x
				&& parent->y + y + height > other->parent->y + other->y
				&& parent->x + x < other->parent->x + other->x + other->width
				&& parent->y + y < other->parent->y + other->y + other->height;*/
	}

	/*
	bool collideHitbox(Mask* current, Mask* other)
	{
		return current->parent->x - current->parent->originX + current->parent->width > other->parent->x - other->parent->originX
				&& current->parent->y - current->parent->originY + current->parent->height > other->parent->y - other->parent->originY
				&& current->parent->x - current->parent->originX < other->parent->x - other->parent->originX + other->parent->width
				&& current->parent->y - current->parent->originY < other->parent->y - other->parent->originY + other->parent->height;
	}
	*/

	void Hitbox::update()
	{
		if (parent)
		{
			// update entity bounds
			parent->originX = -x;
			parent->originY = -y;
			parent->width = width;
			parent->height = height;
		}
	}
}