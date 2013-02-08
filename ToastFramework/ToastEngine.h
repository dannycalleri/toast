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

#ifndef TOASTENGINE_H
#define TOASTENGINE_H

#include "ToastCommon.h"
#include "ToastGraphics.h"
#include "ToastEventsManager.h"

namespace Toast
{
	class Engine : public EventManager
	{
	public:
		/**
		*	If you decide to go fullscreen, the width and height parameters
		*	will be ignored at the moment and you'll go fullscreen at your
		*	native resolution
		*/
		Engine(unsigned int width, unsigned int height, bool fullscreen=false);
		virtual ~Engine();

		ALLEGRO_DISPLAY* display;
		TFRandom*	  random;

		// run the engine main loop
		int run();

		void update();
		void render();
		void cleanUp();

		// Overriden event handlers
		virtual void checkEvents();
		virtual void onExit();

		// reset rendering buffer to the
		// the engine's rendering buffer
		void resetRenderingBuffer();

	protected:
		bool fullscreen;
		bool paused;
		bool isRunning;
		unsigned int width;
		unsigned int height;

		ALLEGRO_BITMAP* buffer; // screen buffer for rendering
		float scaleW;
		float scaleH;
		float scaleX;
		float scaleY;

		// =====================
		// TIMING STUFF
		double prevTime;
		double time;

		// FPS update
		float frameUpdateTimer;
		float frameCounter;
		// =====================

		// init the engine
		bool initEngine(); 
		void updateEngineStats();
	};
}

#endif // TOASTENGINE_H