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
#include <vector>

namespace Toast
{
	class Cat;
	class Emitter;
	class Room;
	class Spritemap;

	// x, y is the middle of the kid's feet; the hitbox covers his feet only,
	// so his head can overlap a wall above him, as in a top-down game
	class Player : public Entity
	{
	public:
		static const float SPEED;
		static const float FIRE_COOLDOWN;
		static const float MOUTH_SECONDS;
		// in half hearts
		static const int MAX_HEALTH = 6;
		static const float INVULNERABLE_SECONDS;

		Player(float x, float y);

		virtual void update();

		void move(float dx, float dy);
		bool overlapsSolid(float atX, float atY);
		// a touch from something at (fromX, fromY); false while invulnerable
		bool hurt(float fromX, float fromY);
		bool alive() const { return health > 0; }

		Room* room;
		Emitter* effects;
		const std::vector<Cat*>* cats;
		Direction facing;
		int shots;
		int health;
		float invulnerableTime;

	private:
		void animate(float dx, float dy);
		// (aimX, aimY) is the direction from the mouth; true while the player fires
		bool aim(float& aimX, float& aimY);
		float mouthX(Direction d) const;
		void fire(float aimX, float aimY);

		float cooldown;
		float mouthTime;

		Spritemap* sprite;
	};
}

#endif // PLAYER_H
