/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef EFFECTS_H
#define EFFECTS_H

namespace Toast
{
	class Emitter;

	namespace Effects
	{
		// frames of fx.png, in the order of art/fx.txt
		enum Frame { BULLET = 0, SPIT_DROP = 1, SPARK = 2, DUST = 4, BIG_DUST = 5, RING = 6, SHADOW = 7 };

		extern const char* const SPIT;
		extern const char* const IMPACT;
		extern const char* const SPLASH;
		extern const char* const IMPACT_RING;
		extern const char* const DIRT;
		extern const char* const DIRT_BURST;

		Emitter* create();
	}
}

#endif // EFFECTS_H
