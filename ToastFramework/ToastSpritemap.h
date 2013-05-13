/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTSPRITEMAP_H
#define TOASTSPRITEMAP_H

#include "ToastCommon.h"
#include "ToastImage.h"
#include <string>
#include <map>

namespace Toast
{
	class Spritemap : public Image
	{
	public:
		Spritemap(const std::string& fileName, int frameWidth, int frameHeight);
		virtual ~Spritemap();

		virtual void update();
		virtual void render();

		/**
			Add an animation to the map
		*/
		void add(const std::string& name, int frames[],  unsigned int frameCount, unsigned int frameRate = 0, bool loop = true);
		void play(const std::string& name = "", bool reset = false, unsigned int frame = 0);

	public:
		bool complete;
		int frameWidth;
		int frameHeight;

	protected:

		void updateSourceRect();

		Rectangle* rect;
		unsigned int columns;
		unsigned int rows;
		unsigned int frameCount;
		unsigned int index; // index of the frame for 
							// for the current animation

		unsigned int frame; // current animation frame

		float timer; // animation timer

		Anim* anim;
		std::map<std::string, Anim*> anims;
	};
}

#endif // TOASTSPRITEMAP_H