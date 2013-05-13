/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastInput.h"

namespace Toast
{
	int Input::mouseX = 0;
	int Input::mouseY = 0;
	int Input::mouseWindowX = 0;
	int Input::mouseWindowY = 0;
	float Input::mouseScale = 0.0f;
	float Input::offsetY = 0.0f;
	bool Input::leftMousePressed = false;
	bool Input::leftMouseDown = false;
	bool Input::mouseReleased=false;
	bool Input::pressedKeys[TOAST_KEY_MAX];
	bool Input::downKeys[TOAST_KEY_MAX];

	bool Input::pressed(unsigned int key)
	{
		return pressedKeys[key];
	}

	bool Input::down(unsigned int key)
	{
		return downKeys[key];
	}
}