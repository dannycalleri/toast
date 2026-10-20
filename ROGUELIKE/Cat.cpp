/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Cat.h"
#include "Effects.h"
#include "Movement.h"
#include "Player.h"
#include "Room.h"
#include "ScreenShake.h"
#include "TF.h"
#include "ToastEmitter.h"
#include "ToastSpritemap.h"
#include <cmath>

namespace Toast
{
	const float Cat::SPAWN_SECONDS = 0.9f;
	const float Cat::CROUCH_SECONDS = 0.4f;
	const float Cat::FLASH_SECONDS = 0.08f;

	namespace
	{
		const float ZOOMIE_SPEED = 55.0f;
		const float STALK_SPEED = 30.0f;
		const float LEAP_SPEED = 220.0f;
		const float LEAP_SECONDS = 0.35f;
		const float REST_SECONDS = 0.5f;
		// a pouncer crouches once the kid is this close
		const float POUNCE_RANGE = 90.0f;
		const float SPAWN_SHAKE = 0.3f;
		const float DIRT_INTERVAL = 0.1f;
		const float KNOCKBACK = 6.0f;
		// cat.png: 6 frames, then their white copies
		const int FLASH_OFFSET = 6;
	}

	Cat::Cat(float x, float y, Kind kind, Room* room, Player* target, Emitter* effects, ScreenShake* shake) :
		Entity(x, y),
		kind(kind),
		state(SPAWNING),
		pounce(STALKING),
		stateTime(0.0f),
		lastCrouchSeconds(0.0f),
		hitPoints(HIT_POINTS),
		room(room),
		target(target),
		effects(effects),
		shake(shake),
		age(0.0f),
		flashTime(0.0f),
		dirtTimer(0.0f),
		leapX(x),
		leapY(y)
	{
		Spritemap* shadow = new Spritemap("assets/fx.png", 8, 8);
		int shadowFrame[1] = {Effects::SHADOW};
		shadow->add("still", shadowFrame, 1);
		shadow->play("still");
		shadow->originX = 4;
		shadow->originY = 7;
		shadow->alpha = 0.5f;
		addGraphic(shadow);

		crack = new Spritemap("assets/spawn.png", 16, 16);
		int crackFrames[4] = {0, 1, 2, 3};
		crack->add("open", crackFrames, 4, (unsigned int)(4 / SPAWN_SECONDS + 0.5f), false);
		crack->play("open");
		crack->originX = 8;
		crack->originY = 10;
		addGraphic(crack);

		sprite = new Spritemap(kind == ZOOMIE ? "assets/cat.png" : "assets/cat_pouncer.png", 16, 16);
		struct Animation { const char* name; int frames[2]; unsigned int count; unsigned int rate; };
		const Animation animations[4] = {{"idle", {0, 1}, 2, 3}, {"run", {2, 3}, 2, 10}, {"crouch", {4, 4}, 1, 0}, {"pounce", {5, 5}, 1, 0}};
		for(int i = 0; i < 4; i++)
		{
			const Animation& a = animations[i];
			int frames[2] = {a.frames[0], a.frames[1]};
			int flash[2] = {a.frames[0] + FLASH_OFFSET, a.frames[1] + FLASH_OFFSET};
			sprite->add(a.name, frames, a.count, a.rate);
			sprite->add(std::string(a.name) + "_flash", flash, a.count, a.rate);
		}
		play("idle");
		sprite->originX = 8;
		sprite->originY = 15;
		sprite->visible = false;
		// listed after the shadow and the crack, so it draws over them
		addGraphic(sprite);

		setHitbox(10, 6, 5, 6);
		// cats spawn while the camera slides in, when the world does not update
		Entity::update();
	}

	bool Cat::bodyContains(float px, float py) const
	{
		return px >= x - 7 && px <= x + 7 && py >= y - 12 && py <= y;
	}

	bool Cat::hit(float directionX, float directionY)
	{
		if(!active() || dead())
			return false;

		hitPoints--;
		flashTime = FLASH_SECONDS;
		float length = std::sqrt(directionX * directionX + directionY * directionY);
		if(length > 0)
			Movement::move(*this, room, directionX / length * KNOCKBACK, directionY / length * KNOCKBACK);
		return true;
	}

	void Cat::play(const char* animation)
	{
		sprite->play(flashTime > 0 ? std::string(animation) + "_flash" : std::string(animation));
	}

	void Cat::spawn()
	{
		dirtTimer -= TF::elapsed;
		if(effects && dirtTimer <= 0)
		{
			effects->emit(Effects::DIRT, x, y - 2);
			dirtTimer = DIRT_INTERVAL;
		}

		if(stateTime < SPAWN_SECONDS)
			return;

		state = ACTIVE;
		stateTime = 0;
		crack->visible = false;
		sprite->visible = true;
		if(effects)
		{
			for(int i = 0; i < 8; i++)
				effects->emit(Effects::DIRT_BURST, x, y - 4);
		}
		if(shake)
			shake->add(SPAWN_SHAKE);
	}

	bool Cat::clearPathTo(float targetX, float targetY) const
	{
		float dx = targetX - x;
		float dy = targetY - y;
		int steps = (int)(std::sqrt(dx * dx + dy * dy) / 4.0f);
		for(int i = 1; i < steps; i++)
		{
			if(room && room->isSolidAt(x + dx * i / steps, y - 2 + dy * i / steps))
				return false;
		}
		return true;
	}

	void Cat::steer(float& dx, float& dy)
	{
		if(clearPathTo(target->x, target->y))
			return;

		float stepX, stepY;
		if(!room || !room->nextStep(x, y, target->x, target->y, stepX, stepY))
			return;
		float length = std::sqrt((stepX - x) * (stepX - x) + (stepY - y) * (stepY - y));
		if(length > 0.5f)
		{
			dx = (stepX - x) / length;
			dy = (stepY - y) / length;
		}
	}

	void Cat::walkToward(float targetX, float targetY, float speed)
	{
		float dx = targetX - x;
		float dy = targetY - y;
		float length = std::sqrt(dx * dx + dy * dy);
		if(length < 1.0f)
			return;
		dx /= length;
		dy /= length;
		sprite->flipped = dx > 0;
		if(pounce == STALKING)
			steer(dx, dy);
		Movement::move(*this, room, dx * speed * TF::elapsed, dy * speed * TF::elapsed);
	}

	void Cat::zoom()
	{
		float dx = target->x - x;
		float dy = target->y - y;
		float length = std::sqrt(dx * dx + dy * dy);
		if(length < 1.0f)
			return;
		dx /= length;
		dy /= length;

		// sideways wobble: the zig-zag of a cat that cannot run straight
		float wobble = std::sin(age * 9.0f) * 0.9f;
		float moveX = dx - dy * wobble;
		float moveY = dy + dx * wobble;
		float moveLength = std::sqrt(moveX * moveX + moveY * moveY);
		moveX /= moveLength;
		moveY /= moveLength;
		steer(moveX, moveY);
		Movement::move(*this, room, moveX * ZOOMIE_SPEED * TF::elapsed, moveY * ZOOMIE_SPEED * TF::elapsed);
		sprite->flipped = dx > 0;
		play("run");
	}

	void Cat::setPounce(Pounce next)
	{
		pounce = next;
		stateTime = 0;
	}

	void Cat::stalkAndPounce()
	{
		float dx = target->x - x;
		float dy = target->y - y;
		float distance = std::sqrt(dx * dx + dy * dy);

		switch(pounce)
		{
		case STALKING:
			walkToward(target->x, target->y, STALK_SPEED);
			play("run");
			// a pounce needs a clear line, or the cat leaps into a wall
			if(distance < POUNCE_RANGE && clearPathTo(target->x, target->y))
			{
				leapX = target->x;
				leapY = target->y;
				setPounce(CROUCHING);
			}
			break;

		case CROUCHING:
			play("crouch");
			sprite->flipped = leapX > x;
			if(stateTime >= CROUCH_SECONDS)
			{
				lastCrouchSeconds = stateTime;
				setPounce(LEAPING);
			}
			break;

		case LEAPING:
			play("pounce");
			walkToward(leapX, leapY, LEAP_SPEED);
			if(stateTime >= LEAP_SECONDS)
				setPounce(RESTING);
			break;

		case RESTING:
			play("idle");
			if(stateTime >= REST_SECONDS)
				setPounce(STALKING);
			break;
		}
	}

	void Cat::update()
	{
		age += TF::elapsed;
		stateTime += TF::elapsed;
		flashTime -= TF::elapsed;

		if(state == SPAWNING)
			spawn();
		else if(target && kind == ZOOMIE)
			zoom();
		else if(target)
			stalkAndPounce();

		Entity::update();
	}
}
