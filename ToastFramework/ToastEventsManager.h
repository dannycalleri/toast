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