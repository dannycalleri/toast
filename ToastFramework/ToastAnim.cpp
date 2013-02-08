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