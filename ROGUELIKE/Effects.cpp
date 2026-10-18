/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Effects.h"
#include "ToastEmitter.h"
#include <SDL3/SDL_stdinc.h>

namespace Toast
{
	namespace Effects
	{
		const char* const SPIT = "spit";
		const char* const IMPACT = "impact";
		const char* const SPLASH = "splash";
		const char* const IMPACT_RING = "impact ring";

		Emitter* create()
		{
			const float FULL_TURN = 2.0f * SDL_PI_F;
			Emitter* effects = new Emitter("assets/fx.png", 8, 8);

			effects->newType(SPIT, std::vector<int>(1, SPIT_DROP));
			effects->setMotion(SPIT, 0, 3, 0.15f, FULL_TURN, 4, 0.1f);
			effects->setAlpha(SPIT, 1, 0);

			effects->newType(IMPACT, std::vector<int>(1, SPIT_DROP));
			effects->setMotion(IMPACT, 0, 6, 0.2f, FULL_TURN, 10, 0.15f);
			effects->setScale(IMPACT, 1, 0.3f);
			effects->setGravity(IMPACT, 120);

			effects->newType(SPLASH, std::vector<int>(1, SPARK));
			effects->setMotion(SPLASH, 0, 2, 0.2f, FULL_TURN, 4, 0.1f);
			effects->setAlpha(SPLASH, 1, 0);

			effects->newType(IMPACT_RING, std::vector<int>(1, RING));
			effects->setMotion(IMPACT_RING, 0, 0, 0.18f);
			effects->setScale(IMPACT_RING, 0.3f, 1.2f);
			effects->setAlpha(IMPACT_RING, 1, 0);
			return effects;
		}
	}
}
