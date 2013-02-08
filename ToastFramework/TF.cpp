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

#include "TF.h"
#include "ToastTexture.h"

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
}