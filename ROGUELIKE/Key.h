/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef KEY_H
#define KEY_H

#include "ToastEntity.h"

namespace Toast
{
	class Spritemap;

	class Key : public Entity
	{
	public:
		Key(float x, float y);

		virtual void update();

	private:
		Spritemap* sprite;
		float age;
	};
}

#endif // KEY_H
