/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastEventsManager.h"
#include "ToastInput.h"

namespace Toast
{
	EventManager::EventManager()
	{
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
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			if(ev.type == SDL_EVENT_QUIT || ev.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) 
			{
				this->onExit();
			}
			else if(ev.type == SDL_EVENT_MOUSE_MOTION)
			{
				Input::mouseWindowX = int(ev.motion.x);
				Input::mouseWindowY = int(ev.motion.y);

				SDL_Renderer* renderer = SDL_GetRenderer(SDL_GetWindowFromEvent(&ev));
				if(renderer)
					SDL_ConvertEventToRenderCoordinates(renderer, &ev);

				Input::mouseX = int(ev.motion.x);
				Input::mouseY = int(ev.motion.y);
			}
			else if(ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
				if(ev.button.button == SDL_BUTTON_LEFT)
				{
					Input::leftMousePressed=true;
					Input::leftMouseDown=true;
					Input::mouseReleased=false;
				}
			}
			else if(ev.type == SDL_EVENT_MOUSE_BUTTON_UP)
			{
				if(ev.button.button == SDL_BUTTON_LEFT)
				{
					Input::leftMouseDown=false;
					Input::mouseReleased=true;
				}
			}
			else if(ev.type == SDL_EVENT_KEY_DOWN)
			{
				// key repeat would make pressed() fire
				// again while the key is held
				if(ev.key.repeat)
					continue;

				unsigned int key = Input::keyFromScancode(ev.key.scancode);
				if(key)
				{
					Input::pressedKeys[key] = true;
					Input::downKeys[key] = true;
				}
			}
			else if(ev.type == SDL_EVENT_KEY_UP)
			{
				unsigned int key = Input::keyFromScancode(ev.key.scancode);
				if(key)
					Input::downKeys[key] = false;
			}
		}
	}
}
