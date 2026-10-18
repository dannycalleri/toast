/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef BULLET_H
#define BULLET_H

#include "ToastEntity.h"

namespace Toast
{
	class Emitter;
	class Room;

	class Bullet : public Entity
	{
	public:
		static const float SPEED;
		static const float RANGE;

		// (directionX, directionY) need not be normalised
		Bullet(float x, float y, float directionX, float directionY, Room* room, Emitter* effects);

		virtual void update();

		float velocityX;
		float velocityY;
		float travelled;

	private:
		void burst(const char* particle, int count);

		Room* room;
		Emitter* effects;
	};
}

#endif // BULLET_H
