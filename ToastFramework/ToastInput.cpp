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
	bool Input::leftMousePressed = false;
	bool Input::leftMouseDown = false;
	bool Input::mouseReleased=false;
	bool Input::pressedKeys[TOAST_KEY_MAX];
	bool Input::downKeys[TOAST_KEY_MAX];

	bool Input::pressed(unsigned int key)
	{
		return key < TOAST_KEY_MAX && pressedKeys[key];
	}

	bool Input::down(unsigned int key)
	{
		return key < TOAST_KEY_MAX && downKeys[key];
	}

	unsigned int Input::keyFromScancode(SDL_Scancode sc)
	{
		if(sc >= SDL_SCANCODE_A && sc <= SDL_SCANCODE_Z)
			return TOAST_KEY_A + (sc - SDL_SCANCODE_A);
		// SDL orders the digit rows 1..9,0 while Toast orders them 0..9
		if(sc >= SDL_SCANCODE_1 && sc <= SDL_SCANCODE_9)
			return TOAST_KEY_1 + (sc - SDL_SCANCODE_1);
		if(sc >= SDL_SCANCODE_KP_1 && sc <= SDL_SCANCODE_KP_9)
			return TOAST_KEY_PAD_1 + (sc - SDL_SCANCODE_KP_1);
		if(sc >= SDL_SCANCODE_F1 && sc <= SDL_SCANCODE_F12)
			return TOAST_KEY_F1 + (sc - SDL_SCANCODE_F1);

		switch(sc)
		{
		case SDL_SCANCODE_0:			return TOAST_KEY_0;
		case SDL_SCANCODE_KP_0:			return TOAST_KEY_PAD_0;
		case SDL_SCANCODE_ESCAPE:		return TOAST_KEY_ESCAPE;
		case SDL_SCANCODE_GRAVE:		return TOAST_KEY_TILDE;
		case SDL_SCANCODE_MINUS:		return TOAST_KEY_MINUS;
		case SDL_SCANCODE_EQUALS:		return TOAST_KEY_EQUALS;
		case SDL_SCANCODE_BACKSPACE:	return TOAST_KEY_BACKSPACE;
		case SDL_SCANCODE_TAB:			return TOAST_KEY_TAB;
		case SDL_SCANCODE_LEFTBRACKET:	return TOAST_KEY_OPENBRACE;
		case SDL_SCANCODE_RIGHTBRACKET:	return TOAST_KEY_CLOSEBRACE;
		case SDL_SCANCODE_RETURN:		return TOAST_KEY_ENTER;
		case SDL_SCANCODE_SEMICOLON:	return TOAST_KEY_SEMICOLON;
		case SDL_SCANCODE_APOSTROPHE:	return TOAST_KEY_QUOTE;
		case SDL_SCANCODE_BACKSLASH:	return TOAST_KEY_BACKSLASH;
		case SDL_SCANCODE_NONUSBACKSLASH: return TOAST_KEY_BACKSLASH2;
		case SDL_SCANCODE_COMMA:		return TOAST_KEY_COMMA;
		case SDL_SCANCODE_PERIOD:		return TOAST_KEY_FULLSTOP;
		case SDL_SCANCODE_SLASH:		return TOAST_KEY_SLASH;
		case SDL_SCANCODE_SPACE:		return TOAST_KEY_SPACE;
		case SDL_SCANCODE_INSERT:		return TOAST_KEY_INSERT;
		case SDL_SCANCODE_DELETE:		return TOAST_KEY_DELETE;
		case SDL_SCANCODE_HOME:			return TOAST_KEY_HOME;
		case SDL_SCANCODE_END:			return TOAST_KEY_END;
		case SDL_SCANCODE_PAGEUP:		return TOAST_KEY_PGUP;
		case SDL_SCANCODE_PAGEDOWN:		return TOAST_KEY_PGDN;
		case SDL_SCANCODE_LEFT:			return TOAST_KEY_LEFT;
		case SDL_SCANCODE_RIGHT:		return TOAST_KEY_RIGHT;
		case SDL_SCANCODE_UP:			return TOAST_KEY_UP;
		case SDL_SCANCODE_DOWN:			return TOAST_KEY_DOWN;
		case SDL_SCANCODE_KP_DIVIDE:	return TOAST_KEY_PAD_SLASH;
		case SDL_SCANCODE_KP_MULTIPLY:	return TOAST_KEY_PAD_ASTERISK;
		case SDL_SCANCODE_KP_MINUS:		return TOAST_KEY_PAD_MINUS;
		case SDL_SCANCODE_KP_PLUS:		return TOAST_KEY_PAD_PLUS;
		case SDL_SCANCODE_KP_PERIOD:	return TOAST_KEY_PAD_DELETE;
		case SDL_SCANCODE_KP_ENTER:		return TOAST_KEY_PAD_ENTER;
		case SDL_SCANCODE_KP_EQUALS:	return TOAST_KEY_PAD_EQUALS;
		case SDL_SCANCODE_PRINTSCREEN:	return TOAST_KEY_PRINTSCREEN;
		case SDL_SCANCODE_PAUSE:		return TOAST_KEY_PAUSE;
		case SDL_SCANCODE_LSHIFT:		return TOAST_KEY_LSHIFT;
		case SDL_SCANCODE_RSHIFT:		return TOAST_KEY_RSHIFT;
		case SDL_SCANCODE_LCTRL:		return TOAST_KEY_LCTRL;
		case SDL_SCANCODE_RCTRL:		return TOAST_KEY_RCTRL;
		case SDL_SCANCODE_LALT:			return TOAST_KEY_ALT;
		case SDL_SCANCODE_RALT:			return TOAST_KEY_ALTGR;
		case SDL_SCANCODE_LGUI:			return TOAST_KEY_LWIN;
		case SDL_SCANCODE_RGUI:			return TOAST_KEY_RWIN;
		case SDL_SCANCODE_APPLICATION:	return TOAST_KEY_MENU;
		case SDL_SCANCODE_SCROLLLOCK:	return TOAST_KEY_SCROLLLOCK;
		case SDL_SCANCODE_NUMLOCKCLEAR:	return TOAST_KEY_NUMLOCK;
		case SDL_SCANCODE_CAPSLOCK:		return TOAST_KEY_CAPSLOCK;
		default:						return 0;
		}
	}
}
