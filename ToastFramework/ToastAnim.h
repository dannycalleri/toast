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