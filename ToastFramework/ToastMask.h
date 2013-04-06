/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTMASK_H
#define TOASTMASK_H

#include "ToastCommon.h"

namespace Toast
{
	class Mask
	{
	public:
		Mask();
		virtual ~Mask();

		/**
		*	Entity to which this mask belongs to
		*/
		Entity* parent;
	};
}

#endif // BITLIGHMASK_H