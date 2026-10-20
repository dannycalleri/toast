/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef BULLET_H
#define BULLET_H

#include "ToastEntity.h"
#include <vector>

namespace Toast
{
	class Cat;
	class Emitter;
	class Room;

	class Bullet : public Entity
	{
	public:
		static const float SPEED;
		static const float RANGE;

		// (directionX, directionY) need not be normalised
		Bullet(float x, float y, float directionX, float directionY, Room* room, Emitter* effects,
			   const std::vector<Cat*>* cats = NULL);

		virtual void update();

		float velocityX;
		float velocityY;
		float travelled;

	private:
		void burst(const char* particle, int count);
		Cat* catHit() const;
		void breakApart();

		Room* room;
		Emitter* effects;
		const std::vector<Cat*>* cats;
	};
}

#endif // BULLET_H
