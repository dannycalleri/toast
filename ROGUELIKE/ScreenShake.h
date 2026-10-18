/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef SCREENSHAKE_H
#define SCREENSHAKE_H

#include <random>

namespace Toast
{
	// Trauma-based shake: events add trauma, trauma decays, and the offset
	// grows with trauma squared, so small hits barely move the screen.
	class ScreenShake
	{
	public:
		static const float DECAY_PER_SECOND;
		static const float MAX_OFFSET;

		ScreenShake();

		void add(float amount);
		void update(float seconds);
		void offset(float& x, float& y);

		float trauma;

	private:
		std::mt19937 rng;
	};
}

#endif // SCREENSHAKE_H
