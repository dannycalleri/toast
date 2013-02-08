/*
=========================================================================

Toast 2D game framework
Copyright (C)2012 Danny Calleri

Toast 2D game framework is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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