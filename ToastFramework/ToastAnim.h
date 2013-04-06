/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTANIM_H
#define TOASTANIM_H

#include "ToastCommon.h"
#include <string>
#include <vector>
#include <algorithm>

namespace Toast
{
	class Anim
	{
	public:
		Anim(const std::string& name, int frames[], unsigned int frameCount, unsigned int frameRate = 0, bool loop = true);
		virtual ~Anim();

	public:
		std::string name;
		std::vector<int> frames;
		unsigned int frameCount;
		unsigned int frameRate;
		bool loop;
	};
}

#endif // TOASTANIM_H