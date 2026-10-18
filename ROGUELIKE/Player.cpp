/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Player.h"
#include "Room.h"
#include "TF.h"
#include "ToastInput.h"
#include "ToastSpritemap.h"
#include <cmath>

namespace Toast
{
	const float Player::SPEED = 80.0f;

	namespace
	{
		// kid.png: one row per direction (down, up, side), 4 frames each
		const int FRAMES_PER_ROW = 4;

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
		facing(DOWN)
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
			sprite->add(std::string(names[row]) + "_idle", idle, 1);
			sprite->add(std::string(names[row]) + "_walk", walk, 4, 8);
		}
		sprite->play("down_idle");
		graphic = sprite;

		setHitbox(10, 6, 5, 6);
	}

	bool Player::overlapsSolid(float atX, float atY)
	{
		if(!room)
			return false;
		float left = atX - originX;
		float top = atY - originY;
		// the hitbox is smaller than a tile, so its corners are enough
		float right = left + width - 0.01f;
		float bottom = top + height - 0.01f;
		return room->isSolidAt(left, top) || room->isSolidAt(right, top) ||
			   room->isSolidAt(left, bottom) || room->isSolidAt(right, bottom);
	}

	void Player::moveAxis(float& position, float delta)
	{
		// at most a pixel per step, so a long move (a slow frame) cannot jump a wall
		float start = position;
		int steps = (int)std::ceil(std::fabs(delta));
		for(int i = 1; i <= steps; i++)
		{
			float previous = position;
			position = start + delta * i / steps;
			if(overlapsSolid(x, y))
			{
				position = previous;
				return;
			}
		}
	}

	void Player::move(float dx, float dy)
	{
		moveAxis(x, dx);
		moveAxis(y, dy);
	}

	void Player::animate(float dx, float dy)
	{
		if(dx != 0 || dy != 0)
		{
			if(std::fabs(dx) > std::fabs(dy))
				facing = dx < 0 ? LEFT : RIGHT;
			else
				facing = dy < 0 ? UP : DOWN;
		}

		const char* names[3] = {"down", "up", "side"};
		std::string name = std::string(names[rowOf(facing)]) + (dx != 0 || dy != 0 ? "_walk" : "_idle");
		sprite->play(name);
		sprite->flipped = facing == RIGHT;
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
		animate(dx, dy);
		Entity::update();
	}
}
