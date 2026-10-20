/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Movement.h"
#include "Room.h"
#include <cmath>

namespace Toast
{
	namespace Movement
	{
		namespace
		{
			// at most a pixel per step, so a long move (a slow frame) cannot jump a wall
			void moveAxis(Entity& entity, Room* room, float& position, float delta)
			{
				float start = position;
				int steps = (int)std::ceil(std::fabs(delta));
				for(int i = 1; i <= steps; i++)
				{
					float previous = position;
					position = start + delta * i / steps;
					if(overlapsSolid(entity, room, entity.x, entity.y))
					{
						position = previous;
						return;
					}
				}
			}
		}

		bool overlapsSolid(const Entity& entity, Room* room, float atX, float atY)
		{
			if(!room)
				return false;
			float left = atX - entity.originX;
			float top = atY - entity.originY;
			float right = left + entity.width - 0.01f;
			float bottom = top + entity.height - 0.01f;
			return room->isSolidAt(left, top) || room->isSolidAt(right, top) ||
				   room->isSolidAt(left, bottom) || room->isSolidAt(right, bottom);
		}

		void move(Entity& entity, Room* room, float dx, float dy)
		{
			moveAxis(entity, room, entity.x, dx);
			moveAxis(entity, room, entity.y, dy);
		}
	}
}
