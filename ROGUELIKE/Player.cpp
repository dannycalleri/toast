/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Player.h"
#include "Bullet.h"
#include "Effects.h"
#include "Movement.h"
#include "Room.h"
#include "TF.h"
#include "ToastInput.h"
#include "ToastEmitter.h"
#include "ToastSpritemap.h"
#include "ToastWorld.h"
#include <cmath>

namespace Toast
{
	const float Player::SPEED = 80.0f;
	const float Player::FIRE_COOLDOWN = 0.25f;
	const float Player::MOUTH_SECONDS = 0.1f;

	namespace
	{
		// kid.png: one row per direction (down, up, side), 4 frames each
		const int FRAMES_PER_ROW = 4;
		const int MOUTH_FRAME = 3;
		// from the feet
		const float MOUTH_Y = -6.0f;
		const float MOUTH_SIDE_X = 2.0f;

		Direction dominant(float dx, float dy)
		{
			if(std::fabs(dx) > std::fabs(dy))
				return dx < 0 ? LEFT : RIGHT;
			return dy < 0 ? UP : DOWN;
		}

		int rowOf(Direction d)
		{
			switch(d)
			{
			case DOWN: return 0;
			case UP:   return 1;
			default:   return 2;
			}
		}
	}

	Player::Player(float x, float y) :
		Entity(x, y),
		room(NULL),
		effects(NULL),
		cats(NULL),
		facing(DOWN),
		shots(0),
		cooldown(0.0f),
		mouthTime(0.0f)
	{
		sprite = new Spritemap("assets/kid.png", 16, 16);
		sprite->originX = 8;
		sprite->originY = 15;

		const char* names[3] = {"down", "up", "side"};
		for(int row = 0; row < 3; row++)
		{
			int first = row * FRAMES_PER_ROW;
			int idle[1] = {first};
			int walk[4] = {first + 1, first, first + 2, first};
			int mouth[1] = {first + MOUTH_FRAME};
			sprite->add(std::string(names[row]) + "_idle", idle, 1);
			sprite->add(std::string(names[row]) + "_walk", walk, 4, 8);
			sprite->add(std::string(names[row]) + "_mouth", mouth, 1);
		}
		sprite->play("down_idle");
		graphic = sprite;

		setHitbox(10, 6, 5, 6);
	}

	bool Player::overlapsSolid(float atX, float atY)
	{
		return Movement::overlapsSolid(*this, room, atX, atY);
	}

	void Player::move(float dx, float dy)
	{
		Movement::move(*this, room, dx, dy);
	}

	void Player::animate(float dx, float dy)
	{
		bool walking = dx != 0 || dy != 0;
		if(walking && mouthTime <= 0)
			facing = dominant(dx, dy);

		const char* names[3] = {"down", "up", "side"};
		const char* state = mouthTime > 0 ? "_mouth" : walking ? "_walk" : "_idle";
		sprite->play(std::string(names[rowOf(facing)]) + state);
		sprite->flipped = facing == RIGHT;
	}

	float Player::mouthX(Direction d) const
	{
		if(d == LEFT) return x - MOUTH_SIDE_X;
		if(d == RIGHT) return x + MOUTH_SIDE_X;
		return x;
	}

	bool Player::aim(float& aimX, float& aimY)
	{
		aimX = 0;
		aimY = 0;
		if(Input::leftMouseDown)
		{
			float cursorX = Input::mouseX + TF::camera->x;
			float cursorY = Input::mouseY + TF::camera->y;
			// the side of the mouth depends on where the kid will face
			Direction d = dominant(cursorX - x, cursorY - (y + MOUTH_Y));
			aimX = cursorX - mouthX(d);
			aimY = cursorY - (y + MOUTH_Y);
			return aimX != 0 || aimY != 0;
		}
		if(Input::down(TOAST_KEY_LEFT)) aimX -= 1;
		if(Input::down(TOAST_KEY_RIGHT)) aimX += 1;
		if(Input::down(TOAST_KEY_UP)) aimY -= 1;
		if(Input::down(TOAST_KEY_DOWN)) aimY += 1;
		return aimX != 0 || aimY != 0;
	}

	void Player::fire(float aimX, float aimY)
	{
		facing = dominant(aimX, aimY);
		mouthTime = MOUTH_SECONDS;
		shots++;

		float fromX = mouthX(facing);
		float fromY = y + MOUTH_Y;
		TF::world->add(new Bullet(fromX, fromY, aimX, aimY, room, effects, cats));
		if(effects)
		{
			for(int i = 0; i < 3; i++)
				effects->emit(Effects::SPIT, fromX, fromY);
		}
	}

	void Player::update()
	{
		float dx = 0;
		float dy = 0;
		if(Input::down(TOAST_KEY_A)) dx -= 1;
		if(Input::down(TOAST_KEY_D)) dx += 1;
		if(Input::down(TOAST_KEY_W)) dy -= 1;
		if(Input::down(TOAST_KEY_S)) dy += 1;

		if(dx != 0 && dy != 0)
		{
			dx *= 0.70710678f;
			dy *= 0.70710678f;
		}

		move(dx * SPEED * TF::elapsed, dy * SPEED * TF::elapsed);

		mouthTime -= TF::elapsed;
		cooldown -= TF::elapsed;
		float aimX, aimY;
		if(aim(aimX, aimY) && cooldown <= 0)
		{
			fire(aimX, aimY);
			cooldown = FIRE_COOLDOWN;
		}

		animate(dx, dy);
		Entity::update();
	}
}
