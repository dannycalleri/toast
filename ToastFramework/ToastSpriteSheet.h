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

#ifndef TOASTSPRITESHEET_H
#define TOASTSPRITESHEET_H

namespace Toast
{
	class SpriteSheet
	{
	public:
		SpriteSheet(char* fileName, int width, int height, int tileWidth, int tileHeight);
		~SpriteSheet();

		char* fileName;
		int width;
		int height;
		int tileWidth;
		int tileHeight;
	};
}

#endif // TOASTSPRITESHEET_H