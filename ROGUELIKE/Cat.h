/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef CAT_H
#define CAT_H

#include "ToastEntity.h"

namespace Toast
{
	class Emitter;
	class Player;
	class Room;
	class ScreenShake;
	class Spritemap;

	// x, y is the middle of the cat's feet, like the kid's.
	class Cat : public Entity
	{
	public:
		enum Kind { ZOOMIE, POUNCER };
		enum State { SPAWNING, ACTIVE };
		enum Pounce { STALKING, CROUCHING, LEAPING, RESTING };

		static const float SPAWN_SECONDS;
		static const float CROUCH_SECONDS;

		Cat(float x, float y, Kind kind, Room* room, Player* target, Emitter* effects, ScreenShake* shake);

		// a spawning cat can neither hurt the kid nor be hit
		bool active() const { return state == ACTIVE; }
		// the drawn body, taller than the feet hitbox, for bullets
		bool bodyContains(float px, float py) const;

		virtual void update();

		Kind kind;
		State state;
		Pounce pounce;
		float stateTime;
		float lastCrouchSeconds;

	private:
		void spawn();
		void zoom();
		void stalkAndPounce();
		void walkToward(float targetX, float targetY, float speed);
		// (dx, dy) toward the target, or along a path when a wall is in the way
		void steer(float& dx, float& dy);
		bool clearPathTo(float targetX, float targetY) const;
		void setPounce(Pounce next);

		Room* room;
		Player* target;
		Emitter* effects;
		ScreenShake* shake;
		Spritemap* sprite;
		Spritemap* crack;
		float age;
		float dirtTimer;
		float leapX;
		float leapY;
	};
}

#endif // CAT_H
