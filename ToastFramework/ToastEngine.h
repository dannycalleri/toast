/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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
		*	width and height are the game's logical resolution.
		*	The window is scaled to fit the screen and the game
		*	is letterboxed inside it, in windowed and fullscreen mode
		*/
		Engine(unsigned int width, unsigned int height, bool fullscreen=false);
		virtual ~Engine();

		SDL_Window*   window;
		SDL_Renderer* renderer;
		TFRandom*	  random;

		// run the engine main loop
		int run();

		void update();
		void render();

		// Overriden event handlers
		virtual void checkEvents();
		virtual void onExit();

		// makes the window the render target again
		void resetRenderingBuffer();

	protected:
		bool fullscreen;
		bool paused;
		bool isRunning;
		unsigned int width;
		unsigned int height;

		// =====================
		// TIMING STUFF
		Uint64 prevTime;

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
