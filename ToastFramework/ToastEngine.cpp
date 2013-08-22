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
#include <iostream>
#include <algorithm>
using namespace std;

//#define TOAST_OPENGL
#define TOAST_DIRECT3D

namespace Toast
{
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
		//this->screen = NULL;
		//this->renderer = NULL;
		this->random = NULL;

		this->prevTime = 0;
		this->time = 0;

		this->frameUpdateTimer = 0.0f;
		this->frameCounter = 0;

		this->buffer=0;
		this->scaleH=0.0f;
		this->scaleW=0.0f;
		this->scaleX=0.0f;
		this->scaleY=0.0f;

		// Init Window, Renderer, Random numbers, etc
		if(!this->initEngine())
		{
			TOAST_ERROR("ERROR: Init engine error!");
		}
	}

	Engine::~Engine()
	{
		delete random;
		delete TF::camera;
		delete TF::world;
		delete TF::font;

		// delete all textures!
		TF::destroyAllTextures();

		delete TF::console;

		// debug
		system("pause");
	}

	int Engine::run()
	{

		// Run the game engine :)
		while(this->isRunning)
		{
			checkEvents();

			//time = SDL_GetTicks();
			time = al_get_time();


			TF::elapsed = time - prevTime;//) / 1000.0f;

			//std::cout << "ELAPSED TIME: " << TF::elapsed << "\n";

			// ====================
			updateEngineStats();
			// ====================

			// updating and rendering the current
			// world
			this->update();
			this->render();

			prevTime = time;
		}

		cleanUp();

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

	void Engine::resetRenderingBuffer()
	{
		al_set_target_bitmap(this->buffer);
	}

	void Engine::render()
	{

		al_set_target_bitmap(this->buffer);

		// clearing window with a new color
		Graphics::Clear(0, 0, 0);


		if(TF::world->visible)
			TF::world->render();

		if(TF::debug)
			TF::console->render();


		al_set_target_backbuffer(display);

		Graphics::Clear(0,0,0);

		//std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleW: " << scaleW << " scaleH: " << scaleH << "\n";
		al_draw_scaled_bitmap(buffer, 0, 0, float(this->width), float(this->height), scaleX, scaleY, scaleW, scaleH, 0);

		// present buffer
		Graphics::Present();
	}

	void Engine::updateEngineStats()
	{
		frameUpdateTimer += TF::elapsed;
		frameCounter++;

		if(frameUpdateTimer >= 1.0f)
		{
			TF::FPS = frameCounter;

			std::cout << "FPS : " << TF::FPS << "\n";

			frameUpdateTimer = 0.0f;
			frameCounter = 0;
		}
	}

	bool Engine::initEngine()
	{
		display = NULL;

		// initializing allegro library
		if(!al_init()) 
		{	
			al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);

			return false;
		}

		if(!al_install_mouse()) 
		{
			al_show_native_message_box(display, "Error", "Error", "Failed to initialize the mouse!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		if(!al_install_keyboard()) 
		{
			al_show_native_message_box(display, "Error", "Error", "Failed to initialize the keyboard!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		if(!al_init_image_addon()) 
		{
			al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		if(!al_init_primitives_addon())
		{
			al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_primitive_addon!", 
									 NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		al_init_font_addon(); // initialize the font addon
		al_init_ttf_addon();  // initialize the ttf (True Type Font) addon

		// struct to be filled with display information
		// using this to retrieve native resolution
		ALLEGRO_DISPLAY_MODE disp_data;

		if(fullscreen)
		{
			al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);

			// fullscreen mode
#ifdef TOAST_DIRECT3D
			al_set_new_display_flags(ALLEGRO_DIRECT3D | ALLEGRO_FULLSCREEN);
#endif

#ifdef TOAST_OPENGL
			al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN);
#endif

			// linear filtering
			al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

			display = al_create_display(disp_data.width, disp_data.height);

			// creating rendering buffer
			buffer = al_create_bitmap(this->width, this->height);

			// scale factors
			float sx = disp_data.width / float(this->width);
			float sy = disp_data.height / float(this->height);

			float scale = min(sx, sy);

			// calculating how much the buffer should be scaled
			scaleW = this->width * scale;
			scaleH = this->height * scale;
			scaleX = (disp_data.width - scaleW)/2.0f;
			scaleY = (disp_data.height - scaleH)/2.0f;
		}
		else
		{
			//al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);

			float windowWidth = 1024.0f;
			float windowHeight = 768.0f;

			// full screen window
#ifdef TOAST_DIRECT3D
			al_set_new_display_flags(ALLEGRO_DIRECT3D | ALLEGRO_FULLSCREEN_WINDOW);
#endif

#ifdef TOAST_OPENGL
			al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN_WINDOW);
#endif
			
			// linear filtering
			al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

			display = al_create_display(windowWidth, windowHeight);

			al_set_window_title(al_get_current_display(), "Toast Engine");
			al_set_window_position(al_get_current_display(),20,20);

			// creating rendering buffer
			buffer = al_create_bitmap(this->width, this->height);

			// scale factors
			float sx = windowWidth / float(this->width);
			float sy = windowHeight / float(this->height);

			float scale = min(sx, sy);

			// calculating how much the buffer should be scaled
			scaleW = this->width * scale;
			scaleH = this->height * scale;

			// we divide by two because we want the same
			// to leave the same space between both
			// the sides of the screen
			scaleX = (windowWidth - scaleW)/2.0f;
			scaleY = (windowHeight - scaleH)/2.0f;

			Input::offsetY = scaleY;

			if(scale != 0)
			{
				Input::mouseScale = 1.0f/scale;
			}
		}

		
		if(!display) 
		{
			fprintf(stderr, "failed to create display!\n");
			return false;
		}

		event_queue = al_create_event_queue();
	    if(!event_queue) 
		{
		   fprintf(stderr, "failed to create event_queue!\n");
		   al_destroy_display(display);
		   return false;
	    }

		// registering event queue
		al_register_event_source(event_queue, al_get_display_event_source(display));
		al_register_event_source(event_queue, al_get_mouse_event_source());
		al_register_event_source(event_queue, al_get_keyboard_event_source());

		// clearing window with a new color
		//al_clear_to_color(al_map_rgb(0,255,0));
		//al_clear_to_color(al_map_rgb(255,255,255));
		// present buffer
		//al_flip_display();

		// Init Input
		for(unsigned int i=0; i < TOAST_KEY_MAX; i++)
		{
			Input::pressedKeys[i] = false;
			Input::downKeys[i] = false;
		}

		/*inputManager = new InputManager();
		if(!inputManager)
			return false;*/

		// initializing random number generator
		random = new TFRandom();

		if(random->GetRandomSeed() == 1)
		{
			random->Randomize();
			//std::cout << "RANDOMIZING\n";
		}

		TF::camera = new Point();

		TF::console = new Console();

		TF::font = new Font("04B_03__.TTF", TF::fontSize);

		return true;
	}

	void Engine::checkEvents()
	{
		EventManager::checkEvents();
		/*if(event->type == SDL_QUIT)
			this->isRunning = false;*/
	}

	void Engine::onExit()
	{
		this->isRunning = false;
	}

	void Engine::cleanUp()
	{
		// destroying rendering buffer
		al_destroy_bitmap(buffer);
		al_destroy_display(display);

		//SDL_FreeSurface(displaySurface);
		/*SDL_DestroyRenderer(this->renderer);
		SDL_DestroyWindow(this->screen);
		SDL_Quit();*/
	}
}