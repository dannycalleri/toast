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
#include <iostream>
//#include <string>

namespace Toast
{
	GameWorld::GameWorld()
		:World()
	{
		level = new Level();
		add(level);

		spriteMap = new Spritemap("animazione_loop_goblin.png", 180, 204);
		int idleFrames[] = {0,1,2,3,4,5,6,7,8,9};
		spriteMap->add("IDLE", idleFrames, 10, 15, true);
		spriteMap->play("IDLE");

		spriteMap2 = new Spritemap("animazione_loop_goblin.png", 180, 204);
		int spritemap2IdleFrames[] = {0,1,2,3,4,5,6,7,8,9};
		spriteMap2->add("IDLE", spritemap2IdleFrames, 10, 15, true);
		spriteMap2->play("IDLE");

		demoText = new Text("TOAST FRAMEWORK - DEMO");
		demoText->relative = true;
		addGraphic(demoText);

		pressEnter = new Text("PRESS [ENTER] for Debug mode");
		pressEnter->y = TF::height - pressEnter->height;
		pressEnter->relative = true;
		addGraphic(pressEnter);

		goblin1 = new Entity();
		goblin1->x = 10;
		goblin1->y = 400;
		goblin1->graphic = spriteMap;
		goblin1->setHitbox(spriteMap->frameWidth, spriteMap->frameHeight, 0.0, 0.0);
		goblin1->alpha = 1.0f;
		add(goblin1);

		goblin2 = new Entity();
		goblin2->x = 1000;
		goblin2->y = 400;
		goblin2->setHitboxTo(spriteMap2);
		goblin2->graphic = spriteMap2;
		add(goblin2);

		controlsHelp = new Image("controls.png");
		controlsHelp->x = pressEnter->x + controlsHelp->width / 2;
		controlsHelp->y = pressEnter->y - controlsHelp->height;
		controlsHelp->alpha = 1.0f;
		controlsHelp->relative = true;
		controlsHelp->centerOrigin();
		controlsHelp->angle = -0.05f;
		addGraphic(controlsHelp);

		// goblin2 initially go to the left
		dir=-1;
	}

	GameWorld::~GameWorld()
	{
	}

	void GameWorld::update()
	{
		// calling super-class update
		World::update();

		if(Input::leftMousePressed)
		{
			std::cout << "[PRESSED]\n";
		}

		if(Input::leftMouseDown)
		{
			std::cout << "[DOWN]\n";
		}

		if(Input::down(TOAST_KEY_UP))
		{
			goblin1->y -= 300 * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_DOWN))
		{
			goblin1->y += 300 * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_LEFT))
		{
			goblin1->x -= 300 * TF::elapsed;
			goblin1->graphic->flipped=false;
		}

		if(Input::down(TOAST_KEY_RIGHT))
		{
			goblin1->x += 300 * TF::elapsed;
			goblin1->graphic->flipped=true;
		}

		if(Input::down(TOAST_KEY_A)) 
		{
			TF::camera->x -= 300 * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_D)) 
		{
			TF::camera->x += 300 * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_W)) {
			TF::camera->y -= 300 * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_S)) 
		{
			TF::camera->y += 300 * TF::elapsed;
		}

		if(Input::down(TOAST_KEY_ESCAPE)) 
		{
			TF::engine->onExit();
		}

		// logic for goblin2
		goblin2->x += 300*dir*TF::elapsed;
		if(goblin2->x >= TF::width - goblin2->width)
		{
			goblin2->graphic->flipped = false;
			dir=-1;
		}
		else if(goblin2->x <= 0)
		{
			goblin2->graphic->flipped = true;
			dir=1;
		}

		if(goblin1->collideRect(goblin1->x, goblin1->y, goblin2->x, goblin2->y, goblin2->width, goblin2->height))
		{
			std::cout << "GOBLINS COLLIDING!\n";
		}

		demoText->point->x = TF::camera->x;
		demoText->point->y = TF::camera->y;
		pressEnter->point->x = TF::camera->x;
		pressEnter->point->y = TF::camera->y;
		controlsHelp->point->x = TF::camera->x;
		controlsHelp->point->y = TF::camera->y;
	}
}