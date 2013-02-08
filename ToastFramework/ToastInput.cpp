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

#include "ToastInput.h"

namespace Toast
{
	int Input::mouseX = 0;
	int Input::mouseY = 0;
	int Input::mouseWindowX = 0;
	int Input::mouseWindowY = 0;
	float Input::mouseScaleX = 0.0f;
	float Input::mouseScaleY = 0.0f;
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