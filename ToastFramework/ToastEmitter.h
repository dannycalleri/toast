/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTEMITTER_H
#define TOASTEMITTER_H

#include "ToastCommon.h"
#include "ToastGraphic.h"
#include <map>
#include <set>
#include <string>
#include <vector>

namespace Toast
{
	class Emitter : public Graphic
	{
	public:
		Emitter(const std::string& fileName, int frameWidth, int frameHeight);
		virtual ~Emitter();

		// the particle shows the frames in order over its life
		void newType(const std::string& name, const std::vector<int>& frames);
		// angles in radians, 0 is right and positive is clockwise (y grows down);
		// each value is the minimum, plus a random part of its range
		void setMotion(const std::string& name, float angle, float distance, float duration,
					   float angleRange = 0.0f, float distanceRange = 0.0f, float durationRange = 0.0f);
		void setAlpha(const std::string& name, float from = 1.0f, float to = 0.0f);
		void setScale(const std::string& name, float from = 1.0f, float to = 1.0f);
		// pixels per second squared, downwards
		void setGravity(const std::string& name, float gravity);

		void emit(const std::string& name, float x, float y);

		int particleCount() const { return (int)particles.size(); }
		size_t particleCapacity() const { return particles.capacity(); }
		// relative to the emitter, before the camera
		void particlePosition(int index, float& x, float& y) const;

		virtual void update();
		virtual void render();

	private:
		struct Type
		{
			std::vector<int> frames;
			float angle, angleRange;
			float distance, distanceRange;
			float duration, durationRange;
			float alphaFrom, alphaTo;
			float scaleFrom, scaleTo;
			float gravity;
		};

		struct Particle
		{
			const Type* type;
			float startX, startY;
			float moveX, moveY;
			float time;
			float duration;
		};

		Type* findType(const std::string& name);
		static float progress(const Particle& particle);

		Texture* texture;
		int frameWidth;
		int frameHeight;
		int columns;
		std::map<std::string, Type> types;
		std::set<std::string> reportedNames;
		std::vector<Particle> particles;
	};
}

#endif // TOASTEMITTER_H
