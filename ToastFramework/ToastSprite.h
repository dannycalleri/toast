/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

/**
	WARNING: THIS IS A DEPRECATE CLASS THAT SHOULD BE REMOVED 
		FROM THE FRAMEWORK IN FUTURE VERSIONS.
		YOU SHOULDN'T USE IT!
*/

#ifndef TOASTSPRITE_H
#define TOASTSPRITE_H

#include "ToastCommon.h"
#include <vector>

namespace Toast
{
	class Sprite
	{
	public:
		Sprite(char* fileName, int x, int y, int width, int height);
		~Sprite();

		void render();

	public:
		int x;
		int y;
		int width;
		int height;

	protected:
		char* fileName;
		Texture* image;
		SpriteSheet* spriteSheet;
		int srcX;
		int srcY;
		//std::vector< // animations list

		void init();
	};
}

#endif