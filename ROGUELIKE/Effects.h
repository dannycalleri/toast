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
		enum Frame { BULLET = 0, SPIT_DROP = 1, SPARK = 2, BIG_SPARK = 3, DUST = 4, BIG_DUST = 5, RING = 6, SHADOW = 7, ORANGE_FUR = 8, GREY_FUR = 9, KEY = 13 };

		extern const char* const SPIT;
		extern const char* const IMPACT;
		extern const char* const SPLASH;
		extern const char* const IMPACT_RING;
		extern const char* const DIRT;
		extern const char* const DIRT_BURST;
		extern const char* const FUR;
		extern const char* const GREY_FUR_TUFT;
		extern const char* const SPARKLE;

		Emitter* create();
	}
}

#endif // EFFECTS_H
