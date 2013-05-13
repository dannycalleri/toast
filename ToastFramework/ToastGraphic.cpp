/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastGraphic.h"

namespace Toast
{
	Graphic::Graphic()
	{
		this->active = true;
		this->visible = true;
		this->x = 0.0f;
		this->y = 0.0f;
		this->width=0;
		this->height=0;
		this->scrollX = 1.0f;
		this->scrollY = 1.0f;
		this->relative = false;
		this->point = new Point();
		this->flipped = false;
		this->alpha=1.0f;
	}

	Graphic::~Graphic()
	{
		delete this->point;
	}


}