/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

//============================================
// This class contains static members
// for accessing all the engine subsystems
// and properties
//============================================

#ifndef TF_H
#define TF_H

#include "ToastCommon.h"
#include <map>
#include <string>

namespace Toast
{

	class TF
	{
	public:
		// declarations
		static const float VERSION;
		static unsigned int width;
		static unsigned int height;
		static float halfWidth;
		static float halfHeight;
		static float FPS;

		static bool debug;
		static bool paused;
		static Console* console;

		// default font used by the framework
		static Font* font;
		static unsigned int fontSize;

		// time elapsed since last frame
		static double elapsed;

		static Engine* engine;
		// a point that represents the camera
		static Point* camera;
		// the world currently being updated
		static World* world;

		// a list of all the textures created for the game
		static std::map<std::string, Texture*> textures;

		static Texture* getTexture(std::string str);
		static void addTexture(std::string str, Texture* tex);

		static void destroyAllTextures();
		static void setCamera(float x = 0, float y = 0);
		static void resetCamera();

		// gives a random number between 0 and 1
		static float random();
	};
}

#endif // TF_H