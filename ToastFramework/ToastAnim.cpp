/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastAnim.h"

namespace Toast
{
	Anim::Anim(const std::string& name, int frames[], unsigned int frameCount, unsigned int frameRate, bool loop)
	{
		this->name = name;

		this->frames.clear();
		this->frames.resize(frameCount);
		std::copy(frames, frames + frameCount, this->frames.begin());

		this->frameCount = frameCount;
		this->frameRate = frameRate;
		this->loop = loop;
	}

	Anim::~Anim()
	{
	}
}