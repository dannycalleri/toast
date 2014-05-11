/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTHITBOX_H
#define TOASTHITBOX_H

#include "ToastCommon.h"
#include "ToastMask.h"

namespace Toast
{
	class Hitbox : public Mask
	{
	public:
		Hitbox(int width, int height, float x, float y);
		virtual ~Hitbox();

		virtual void update();

		virtual bool collide(Mask* mask);
		
		bool collideMask(Mask* other);
		bool collideHitbox(Hitbox* other);
		//friend bool collideHitbox(Mask* current, Mask* other);

	public:
		int width;
		int height;
		float x;
		float y;

	/*private:
		
		typedef bool (Hitbox::*CollideFunction)(Hitbox* other);
		typedef std::map<MaskType, CollideFunction> CollideMap;
		CollideMap check;*/
		
	};
}

#endif // TOASTHITBOX_H