/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef DOOR_H
#define DOOR_H

namespace Toast
{
	enum DoorType
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	class Door 
	{
	public:
		Door(float x, float y, DoorType type)
		{
			this->x = x;
			this->y = y;
			this->opened = true;
			this->type = type;
		}

		virtual ~Door() {};

	public:
		float x;
		float y;
		bool opened;
		DoorType type;

	};
}

#endif // DOOR_H