/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "GameWorld.h"
#include "ToastGraphics.h"
#include "ToastInput.h"
#include "ToastEngine.h"
#include "ToastTexture.h"
#include "ToastImage.h"
#include "ToastSpritemap.h"
#include "ToastTilemap.h"
#include "ToastRandom.h"
#include "ToastCanvas.h"
#include "ToastEntity.h"
#include "ToastText.h"
#include "ToastFont.h"

#include "RoomsManager.h"
#include "Dungeon.h"

#include <iostream>

#define CAMERA_SPEED 600

//#include <string>

#include "ToastHitbox.h"

namespace Toast
{
	GameWorld::GameWorld()
		:World()
	{
		font = new Font("assets/pixelfont.TTF", 24);
		roomsManager = new RoomsManager(this);
		dungeon = new Dungeon(this);

		demoText = new Text("TOAST FRAMEWORK - DEMO");
		demoText->font = font;
		demoText->updateCanvas();
		demoText->relative = true;
		addGraphic(demoText);

		pressEnter = new Text("PRESS [ENTER] for Debug mode");
		pressEnter->font = font;
		pressEnter->updateCanvas();
		pressEnter->y = TF::height - pressEnter->height;
		pressEnter->relative = true;
		addGraphic(pressEnter);

		generatingText = new Text("Generating Dungeon...");
		generatingText->font = font;
		generatingText->updateCanvas();
		generatingText->relative = true;
		addGraphic(generatingText);

		generatingText->x = TF::halfWidth - generatingText->width/2;
		generatingText->y = TF::halfHeight - generatingText->height/2;
		generatingText->scrollX = 0;
		generatingText->scrollY = 0;
		generatingText->visible=false;

		generateDungeon=false;

		/*Hitbox* hitbox = new Hitbox(30,30,10,10);
		Hitbox* hitbox2 = new Hitbox(30,30, 10,10);
		Mask* mask = new Mask();
		Mask* mask2 = new Mask();

		hitbox->collide(mask);
		hitbox->collide(hitbox2);
		mask->collide(mask2);
		mask->collide(hitbox);

		delete mask;
		delete hitbox;
		delete hitbox2;
		delete mask2;*/
	}

	GameWorld::~GameWorld()
	{
		TOAST_SAFE_DELETE(font);
		TOAST_SAFE_DELETE(roomsManager);
		TOAST_SAFE_DELETE(dungeon);
	}

	void GameWorld::update()
	{
		// calling super-class update
		World::update();

		if(generateDungeon)
		{
			dungeon->destroy();
			dungeon->generate(10);
			generatingText->visible=false;
			generateDungeon=false;
		}

		if(Input::pressed(TOAST_KEY_G))
		{
			generatingText->visible=true;
			generateDungeon=true;
		}

		if(Input::down(TOAST_KEY_A)) 
		{
			TF::camera->x -= CAMERA_SPEED * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_D)) 
		{
			TF::camera->x += CAMERA_SPEED * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_W)) {
			TF::camera->y -= CAMERA_SPEED * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_S)) 
		{
			TF::camera->y += CAMERA_SPEED * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_ESCAPE)) 
		{
			TF::engine->onExit();
		}

		demoText->point->x = TF::camera->x;
		demoText->point->y = TF::camera->y;
		pressEnter->point->x = TF::camera->x;
		pressEnter->point->y = TF::camera->y;
	}
}