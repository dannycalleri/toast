/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTENTITY_H
#define TOASTENTITY_H

#include "ToastCommon.h"

namespace Toast
{
	class Entity
	{
	public:
		Entity(float x = 0.0f, float y = 0.0f);
		virtual ~Entity();

		void setHitbox(int width, int height, float originX, float originY);
		bool collideRect(float x, float y, float rX, float rY, float rWidth, float rHeight);

		virtual void update();
		void render();

		/**
		* Rendering stuff
		*/
		Graphic* graphic;
		//
		
		bool active;
		bool visible;
		bool collidable;
		float x;
		float y;
		float alpha;

		/**
		* Width and height of the Entity's hitbox
		*/
		int width;
		int height;

		/**
		* Coordinates of the Entity's hitbox
		*/
		float originX;
		float originY;

		/**
		* Collision temp variables
		*/
		float _x;
		float _y;
	};
}

#endif // TOASTENTITY_H