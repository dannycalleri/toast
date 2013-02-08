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

#include "ToastGraphic.h"

namespace Toast
{
	Graphic::Graphic()
	{
		this->active = false;
		this->visible = true;
		this->x = 0.0f;
		this->y = 0.0f;
		this->width=0;
		this->height=0;
		this->scrollX = 1.0f;
		this->scrollY = 1.0f;
		this->relative = true;
		this->point = new Point();
		this->flipped = false;
		this->alpha=1.0f;
	}

	Graphic::~Graphic()
	{
		delete this->point;
	}


}