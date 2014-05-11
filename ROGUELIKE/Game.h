/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef GAMEDEMO_H
#define GAMEDEMO_H

#include "Toast.h"

namespace Toast
{
	class Game : public Engine
	{
	public:
		Game();

	public:
		static const int TILESIZE;
	};
}

#endif