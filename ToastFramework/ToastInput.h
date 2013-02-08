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

#ifndef TOASTINPUTMANAGER_H
#define TOASTINPUTMANAGER_H

#include "ToastCommon.h"

namespace Toast
{
	class Input
	{
	public:

		/**
		*	Mouse coordinates scaled to the
		*	actual game resolution
		*	(use this one to move your in-game
		*	objects respect to the mouse)
		*/
		static int mouseX;
		static int mouseY;

		/**
		*	Mouse coordinates on window
		*/
		static int mouseWindowX;
		static int mouseWindowY;

		static float mouseScaleX;
		static float mouseScaleY;
		static bool leftMousePressed;	// if LMB has been pressed in the current frame
		static bool leftMouseDown;		// if LMB is held down by the user
		static bool mouseReleased;

		// KEYBOARD
		static bool pressedKeys[TOAST_KEY_MAX]; // keys that have been pressed in the current frame
		static bool downKeys[TOAST_KEY_MAX];		// keys held down by the user

		/**
		*	key is a keycode as defined in
		*	(not a modifier)
		*	ToastKeycodes.h
		*/
		static bool pressed(unsigned int key);
		static bool down(unsigned int key);
	};
}

#endif