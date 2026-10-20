/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastEngine.h"
#include "ToastRandom.h"
#include "ToastWorld.h"
#include "TF.h"
#include "ToastInput.h"
#include "ToastConsole.h"
#include "ToastFont.h"
#include <algorithm>
#include <cstdlib>

namespace Toast
{
	const double Engine::MAX_FRAME_SECONDS = 0.1;

	Engine::Engine(unsigned int width, unsigned int height, bool fullscreen)
		: EventManager()
	{
		this->width = width;
		this->height = height;
		TF::width = width;
		TF::height = height;
		TF::halfWidth = (float)width/2;
		TF::halfHeight = (float)height/2;
		TF::engine = this;
		
		this->fullscreen=fullscreen;
		this->paused = false;
		this->isRunning = true;
		this->window = NULL;
		this->renderer = NULL;
		this->random = NULL;

		this->prevTime = 0;

		this->frameUpdateTimer = 0.0f;
		this->frameCounter = 0;

		// Init Window, Renderer, Random numbers, etc
		if(!this->initEngine())
		{
			fatalError(SDL_GetError());
			// game constructors run right after this one and
			// use TF::console and TF::font, which are not created
			std::exit(EXIT_FAILURE);
		}
	}

	Engine::~Engine()
	{
		// everything below owns SDL textures or fonts,
		// so it goes before the renderer and TTF
		delete TF::world;
		TF::world = NULL;
		delete TF::console;
		TF::console = NULL;
		delete TF::font;
		TF::font = NULL;
		TF::destroyAllTextures();

		delete random;
		delete TF::camera;
		TF::camera = NULL;

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();

		TF::engine = NULL;
	}

	int Engine::run()
	{
		prevTime = SDL_GetTicksNS();

		// Run the game engine :)
		while(this->isRunning)
		{
			checkEvents();

			Uint64 time = SDL_GetTicksNS();
			TF::elapsed = frameSeconds(prevTime, time);
			prevTime = time;

			// ====================
			updateEngineStats();
			// ====================

			// updating and rendering the current
			// world
			this->update();
			this->render();
		}

		return 0;
	}

	void Engine::update()
	{
		
		if(TF::world->active && !TF::paused)
		{
			
			TF::world->update();
		}

		if(TF::debug)
			TF::console->update();
	}

	void Engine::windowSize(int logicalWidth, int logicalHeight, int usableWidth, int usableHeight,
							int& windowWidth, int& windowHeight)
	{
		// 80% leaves room for the menu bar, dock and window borders
		float fit = std::min(0.8f * usableWidth / logicalWidth, 0.8f * usableHeight / logicalHeight);
		if(fit < 1.0f)
		{
			windowWidth = int(logicalWidth * fit);
			windowHeight = int(logicalHeight * fit);
			return;
		}
		// whole multiples keep every game pixel the same size on screen
		int scale = int(fit);
		windowWidth = logicalWidth * scale;
		windowHeight = logicalHeight * scale;
	}

	double Engine::frameSeconds(Uint64 prevTicksNS, Uint64 ticksNS)
	{
		return std::min(double(ticksNS - prevTicksNS) / SDL_NS_PER_SECOND, MAX_FRAME_SECONDS);
	}

	void Engine::setPixelArt(bool pixelArt)
	{
		SDL_SetDefaultTextureScaleMode(renderer, pixelArt ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR);
	}

	void Engine::resetRenderingBuffer()
	{
		SDL_SetRenderTarget(renderer, NULL);
	}

	void Engine::render()
	{
		resetRenderingBuffer();

		// also clears the letterbox bars
		Graphics::Clear(0, 0, 0);

		if(TF::world->visible)
			TF::world->render();

		if(TF::debug)
			TF::console->render();

		Graphics::Present();
	}

	void Engine::updateEngineStats()
	{
		frameUpdateTimer += TF::elapsed;
		frameCounter++;

		if(frameUpdateTimer >= 1.0f)
		{
			TF::FPS = frameCounter;

			frameUpdateTimer = 0.0f;
			frameCounter = 0;
		}
	}

	bool Engine::initEngine()
	{
		if(!SDL_Init(SDL_INIT_VIDEO))
			return false;

		if(!TTF_Init())
			return false;

		int windowWidth = this->width;
		int windowHeight = this->height;
		SDL_Rect usable;
		if(SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &usable))
			windowSize(this->width, this->height, usable.w, usable.h, windowWidth, windowHeight);

		SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
		if(fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN;

		if(!SDL_CreateWindowAndRenderer("Toast Engine", windowWidth, windowHeight, flags, &window, &renderer))
			return false;

		if(!SDL_SetRenderLogicalPresentation(renderer, this->width, this->height, SDL_LOGICAL_PRESENTATION_LETTERBOX))
			return false;

		// without it the loop renders as fast as the CPU allows;
		// some drivers refuse it, and the game still runs then
		SDL_SetRenderVSync(renderer, 1);

		// Init Input
		for(unsigned int i=0; i < TOAST_KEY_MAX; i++)
		{
			Input::pressedKeys[i] = false;
			Input::downKeys[i] = false;
		}

		// initializing random number generator
		random = new TFRandom();

		if(random->GetRandomSeed() == 1)
		{
			random->Randomize();
		}

		TF::camera = new Point();

		TF::console = new Console();

		TF::font = new Font("04B_03__.TTF", TF::fontSize);

		return true;
	}

	void Engine::checkEvents()
	{
		EventManager::checkEvents();
	}

	void Engine::onExit()
	{
		this->isRunning = false;
	}
}
