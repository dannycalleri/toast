/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Bullet.h"
#include "Cat.h"
#include "Effects.h"
#include "Room.h"
#include "TF.h"
#include "ToastEmitter.h"
#include "ToastSpritemap.h"
#include "ToastWorld.h"
#include <cmath>

namespace Toast
{
	const float Bullet::SPEED = 180.0f;
	const float Bullet::RANGE = 150.0f;

	Bullet::Bullet(float x, float y, float directionX, float directionY, Room* room, Emitter* effects,
				   const std::vector<Cat*>* cats) :
		Entity(x, y),
		travelled(0.0f),
		room(room),
		effects(effects),
		cats(cats)
	{
		float length = std::sqrt(directionX * directionX + directionY * directionY);
		if(length == 0.0f)
		{
			directionY = 1.0f;
			length = 1.0f;
		}
		velocityX = directionX / length * SPEED;
		velocityY = directionY / length * SPEED;

		Spritemap* sprite = new Spritemap("assets/fx.png", 8, 8);
		int frame[1] = {Effects::BULLET};
		sprite->add("fly", frame, 1);
		sprite->play("fly");
		sprite->originX = 4;
		sprite->originY = 4;
		graphic = sprite;
		setHitbox(4, 4, 2, 2);
	}

	void Bullet::burst(const char* particle, int count)
	{
		if(!effects)
			return;
		for(int i = 0; i < count; i++)
			effects->emit(particle, x, y);
	}

	bool Bullet::hitsCat() const
	{
		if(!cats)
			return false;
		for(size_t i = 0; i < cats->size(); i++)
		{
			if((*cats)[i]->active() && (*cats)[i]->bodyContains(x, y))
				return true;
		}
		return false;
	}

	void Bullet::breakApart()
	{
		burst(Effects::IMPACT, 6);
		burst(Effects::IMPACT_RING, 1);
		TF::world->removeLater(this);
	}

	void Bullet::update()
	{
		// steps of at most a quarter tile, so a slow frame cannot carry it through a wall
		float distance = std::sqrt(velocityX * velocityX + velocityY * velocityY) * TF::elapsed;
		int steps = (int)std::ceil(distance / 4.0f);
		for(int i = 0; i < steps; i++)
		{
			x += velocityX * TF::elapsed / steps;
			y += velocityY * TF::elapsed / steps;
			if((room && room->isSolidAt(x, y)) || hitsCat())
			{
				breakApart();
				return;
			}
		}

		travelled += distance;
		if(travelled >= RANGE)
		{
			burst(Effects::SPLASH, 4);
			TF::world->removeLater(this);
			return;
		}
		Entity::update();
	}
}
