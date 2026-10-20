/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Key.h"
#include "Effects.h"
#include "TF.h"
#include "ToastSpritemap.h"
#include <cmath>

namespace Toast
{
	Key::Key(float x, float y) :
		Entity(x, y),
		age(0.0f)
	{
		sprite = new Spritemap("assets/fx.png", 8, 8);
		int frame[1] = {Effects::KEY};
		sprite->add("still", frame, 1);
		sprite->play("still");
		sprite->originX = 4;
		sprite->originY = 8;
		graphic = sprite;
		setHitbox(8, 6, 4, 6);
		// drawn before its first update when it drops during the kill freeze
		Entity::update();
	}

	void Key::update()
	{
		age += TF::elapsed;
		sprite->y = std::round(std::sin(age * 4.0f) * 2.0f) - 2.0f;
		Entity::update();
	}
}
