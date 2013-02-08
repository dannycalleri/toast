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

#include "ToastEventsManager.h"
#include "ToastInput.h"

#include <iostream>

namespace Toast
{
	EventManager::EventManager()
	{
		this->event_queue=0;
	}

	EventManager::~EventManager()
	{
	}

	void EventManager::checkEvents()
	{
		ALLEGRO_EVENT ev;
		ALLEGRO_TIMEOUT timeout;
		// it waits only a millisecond for an incoming event
		al_init_timeout(&timeout, 0.001);
 
		bool get_event = al_wait_for_event_until(event_queue, &ev, &timeout);
		//al_wait_for_event(event_queue, &ev);
 
		// resetting frame based input stuff
		Input::leftMousePressed=false;
		for(unsigned int i=0; i < TOAST_KEY_MAX; i++)
		{
			Input::pressedKeys[i] = false;
		}


		if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
		{
			this->onExit();
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES ||
				ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{
			Input::mouseWindowX = ev.mouse.x;
			Input::mouseWindowY = ev.mouse.y;

			Input::mouseX = Input::mouseWindowX * Input::mouseScaleX;

			// TODO: REMOVE HARDCODED VALUE!
			Input::mouseY = Input::mouseWindowY * Input::mouseScaleY - 128.0f;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(ev.mouse.button == 1)
			{
				Input::leftMousePressed=true;
				Input::leftMouseDown=true;
				Input::mouseReleased=false;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
		{
			Input::leftMouseDown=false;
			Input::mouseReleased=true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			Input::pressedKeys[ev.keyboard.keycode] = true;
			Input::downKeys[ev.keyboard.keycode] = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			Input::downKeys[ev.keyboard.keycode] = false;
		}
	}
}