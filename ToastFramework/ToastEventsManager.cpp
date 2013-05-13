/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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

			Input::mouseX = Input::mouseWindowX * Input::mouseScale;
			Input::mouseY = (Input::mouseWindowY * Input::mouseScale) - (Input::offsetY * Input::mouseScale);
			
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