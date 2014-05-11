/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "TF.h"
#include "ToastTexture.h"
#include "ToastEngine.h"
#include "ToastRandom.h"

#include <iostream>

namespace Toast
{
	// definitions
	const float TF::VERSION = 1.0;
	unsigned int TF::width = 0;
	unsigned int TF::height = 0;
	float TF::halfWidth = 0.0f;
	float TF::halfHeight = 0.0f;
	double TF::elapsed = 0.0;
	float TF::FPS = 0.0f;

	bool TF::debug = false;
	bool TF::paused = false;

	Console* TF::console = 0;

	Point* TF::camera = 0;
	Engine* TF::engine = 0;
	World* TF::world = 0;

	unsigned int TF::fontSize = 72;
	Font* TF::font = 0;

	std::map<std::string, Texture*> TF::textures;

	Texture* TF::getTexture(std::string str)
	{
		return (Texture*)(TF::textures[str]);
	}

	void TF::addTexture(std::string str, Texture* tex)
	{
		TF::textures[str] = tex;
	}

	void TF::destroyAllTextures()
	{
		std::map<std::string, Texture*>::iterator i;

		for(i = TF::textures.begin(); i != TF::textures.end(); i++)
		{
			std::cout << "[TF destroy] deleting texture " << i->first << "\n";

			// delete texture
			delete i->second;
		}
	}

	void TF::setCamera(float x, float y)
	{
		TF::camera->x = x;
		TF::camera->y = y;
	}

	void TF::resetCamera()
	{
		TF::camera->x = TF::camera->y = 0;
	}

	float TF::random()
	{
		return TF::engine->random->Random();
	}

	float TF::distance(float x1, float y1, float x2, float y2)
	{
		return sqrtf( (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) );
	}
}