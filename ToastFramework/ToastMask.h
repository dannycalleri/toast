/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTMASK_H
#define TOASTMASK_H

#include "ToastCommon.h"
#include <map>

namespace Toast
{
	enum MaskType
	{
		MASK,
		HITBOX
	};

	class Mask
	{
	public:
		Mask();
		virtual ~Mask();

		virtual bool collide(Mask* mask);
		virtual void update();
		virtual void renderDebug();
		void assignTo(Entity* parent);

	//private:
		bool collideMask(Mask* other);

		// pure abstract
		//virtual bool collideHitbox(Hitbox* other)=0;
		

	public:
		/**
		*	Entity to which this mask belongs to
		*/
		Entity* parent;
		MaskType type;

		/*
	private:
		typedef bool (Mask::*CollideFunction)(Mask* other);
		typedef std::map<MaskType, CollideFunction> CollideMap;
		CollideMap check;
		*/
		
	};
}

#endif // TOASTMASK_H