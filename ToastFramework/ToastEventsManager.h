/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTEVENTSMANAGER_H
#define TOASTEVENTSMANAGER_H

#include "ToastCommon.h"

namespace Toast
{
	class EventManager
	{
	public:
		EventManager();
		virtual ~EventManager();

		// it catches every event and call
		// the correct event handler method
		//virtual void checkEvents(SDL_Event* event);

		virtual void checkEvents();

		virtual void onMouseFocus() {};
		virtual void onMouseBlur()  {};
		virtual void onInputFocus() {};
		virtual void onInputBlur()  {};
		virtual void onRestore()	{};
		virtual void onMinimize()	{};
		virtual void onVideoResize(int width, int height) {};
		virtual void onExit() {};
		
	protected:
		// event queue for allegro events
		ALLEGRO_EVENT_QUEUE* event_queue;
	};
}

#endif