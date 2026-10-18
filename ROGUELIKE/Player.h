/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "Maze.h"
#include "ToastEntity.h"

namespace Toast
{
	class Room;
	class Spritemap;

	// x, y is the middle of the kid's feet; the hitbox covers his feet only,
	// so his head can overlap a wall above him, as in a top-down game
	class Player : public Entity
	{
	public:
		static const float SPEED;

		Player(float x, float y);

		virtual void update();

		void move(float dx, float dy);
		bool overlapsSolid(float atX, float atY);

		Room* room;
		Direction facing;

	private:
		void moveAxis(float& position, float delta);
		void animate(float dx, float dy);

		Spritemap* sprite;
	};
}

#endif // PLAYER_H
