/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ScreenShake.h"

namespace Toast
{
	const float ScreenShake::DECAY_PER_SECOND = 1.5f;
	const float ScreenShake::MAX_OFFSET = 6.0f;

	ScreenShake::ScreenShake() :
		trauma(0.0f),
		rng(7)
	{
	}

	void ScreenShake::add(float amount)
	{
		trauma += amount;
		if(trauma > 1.0f)
			trauma = 1.0f;
	}

	void ScreenShake::update(float seconds)
	{
		trauma -= DECAY_PER_SECOND * seconds;
		if(trauma < 0.0f)
			trauma = 0.0f;
	}

	void ScreenShake::offset(float& x, float& y)
	{
		float strength = MAX_OFFSET * trauma * trauma;
		std::uniform_real_distribution<float> unit(-1.0f, 1.0f);
		x = strength * unit(rng);
		y = strength * unit(rng);
	}
}
