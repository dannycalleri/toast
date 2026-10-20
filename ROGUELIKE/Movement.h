/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef MOVEMENT_H
#define MOVEMENT_H

namespace Toast
{
	class Entity;
	class Room;

	// Walking on a room's floor, shared by the kid and the cats. The entity's
	// hitbox must be smaller than a tile.
	namespace Movement
	{
		bool overlapsSolid(const Entity& entity, Room* room, float atX, float atY);
		// x first, then y, so a walker slides along a wall instead of sticking to it
		void move(Entity& entity, Room* room, float dx, float dy);
	}
}

#endif // MOVEMENT_H
