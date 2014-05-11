/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastMask.h"
#include "ToastEntity.h"
#include "ToastHitbox.h"
//#include <functional>
#include <iostream>

namespace Toast
{
	Mask::Mask()
	{
		parent = 0;
		type = MASK;

		//check[type] = &Mask::collideMask;
		//check.insert(std::make_pair(type, &collideMask));
	}

	Mask::~Mask()
	{

	}

	bool Mask::collide(Mask* mask)
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
		}
		
		if(!typeFound && !isColliding)
		{
			isColliding = mask->collide(this);
		}

		return isColliding;


		/*
		if(mask->type == MASK)
		{
			bool isColliding=false;
			isColliding = collideMask(mask);
			return isColliding;
		}

		return false;
		*/

		/*
		bool isColliding=false;
		isColliding = collideMask(this, mask);
		return isColliding;
		*/

		/*
		CollideMap::const_iterator it = check.find(mask->type);
		if(it != check.end()) 
		{
			CollideFunction collideFunction = check[mask->type];
			//collideFunction(mask);
			(reinterpret_cast<CollideFunction>(collideFunction))(mask);
		}
		*/
		/*
		switch(mask->type)
		{
		case MASK:
			{
				return collideMask(mask);

				break;
			}
		case HITBOX:
			{
				Hitbox* hitbox = dynamic_cast<Hitbox*>(mask);
				return hitbox->collideMask(this);

				break;
			}
		}
		*/
	}

	void Mask::update()
	{
	}

	void Mask::renderDebug()
	{
	}

	bool Mask::collideMask(Mask* other)
	{
		std::cout << "[MASK] collideMask" << "\n";
		return false;
		/*
		return parent->x - parent->originX + parent->width > other->parent->x - other->parent->originX
				&& parent->y - parent->originY + parent->height > other->parent->y - other->parent->originY
				&& parent->x - parent->originX < other->parent->x - other->parent->originX + other->parent->width
				&& parent->y - parent->originY < other->parent->y - other->parent->originY + other->parent->height;
				*/
	}

	void Mask::assignTo(Entity* parent)
	{
		this->parent = parent;
		if(parent) update();
	}
}