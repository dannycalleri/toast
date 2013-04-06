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