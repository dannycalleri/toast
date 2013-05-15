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
		// resetting frame based input stuff
		Input::leftMousePressed=false;
		for(unsigned int i=0; i < TOAST_KEY_MAX; i++)
		{
			Input::pressedKeys[i] = false;
		}

		//If any events happen, process all of them this frame.
		while(!al_is_event_queue_empty(event_queue))
		{	
			ALLEGRO_EVENT ev;

			al_wait_for_event(event_queue, &ev);

			if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
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
}