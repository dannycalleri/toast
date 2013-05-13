/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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

		/**
		*	Helpers for calculating mouse
		*	coordinates on screen
		*/
		static float mouseScale;
		static float offsetY;


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