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

#include <cmath>

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
		std::map<std::string, Texture*>::iterator it = TF::textures.find(str);
		return it != TF::textures.end() ? it->second : NULL;
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
			delete i->second;
		}

		TF::textures.clear();
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

	std::string TF::resolvePath(const std::string& path)
	{
		bool absolute = !path.empty() && (path[0] == '/' || path[0] == '\\' || (path.size() > 1 && path[1] == ':'));
		const char* base = SDL_GetBasePath();
		if(absolute || !base)
			return path;
		return std::string(base) + path;
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