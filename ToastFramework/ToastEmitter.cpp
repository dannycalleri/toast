/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastEmitter.h"
#include "ToastError.h"
#include "ToastGraphics.h"
#include "ToastTexture.h"
#include "TF.h"
#include <cmath>

namespace Toast
{
	Emitter::Emitter(const std::string& fileName, int frameWidth, int frameHeight) :
		frameWidth(frameWidth),
		frameHeight(frameHeight),
		columns(1)
	{
		texture = Graphics::LoadTexture(fileName);
		if(texture && frameWidth > 0)
			columns = (int)texture->getWidth() / frameWidth;
		if(columns < 1)
			columns = 1;
	}

	Emitter::~Emitter()
	{
	}

	void Emitter::newType(const std::string& name, const std::vector<int>& frames)
	{
		Type type;
		type.frames = frames.empty() ? std::vector<int>(1, 0) : frames;
		type.angle = 0.0f;
		type.angleRange = 0.0f;
		type.distance = 0.0f;
		type.distanceRange = 0.0f;
		type.duration = 1.0f;
		type.durationRange = 0.0f;
		type.alphaFrom = 1.0f;
		type.alphaTo = 1.0f;
		type.scaleFrom = 1.0f;
		type.scaleTo = 1.0f;
		type.gravity = 0.0f;
		types[name] = type;
	}

	Emitter::Type* Emitter::findType(const std::string& name)
	{
		std::map<std::string, Type>::iterator found = types.find(name);
		if(found != types.end())
			return &found->second;

		if(reportedNames.insert(name).second)
			TOAST_ERROR(("[EMITTER] no particle type named " + name).c_str());
		return NULL;
	}

	void Emitter::setMotion(const std::string& name, float angle, float distance, float duration,
							float angleRange, float distanceRange, float durationRange)
	{
		Type* type = findType(name);
		if(!type)
			return;
		type->angle = angle;
		type->angleRange = angleRange;
		type->distance = distance;
		type->distanceRange = distanceRange;
		type->duration = duration;
		type->durationRange = durationRange;
	}

	void Emitter::setAlpha(const std::string& name, float from, float to)
	{
		Type* type = findType(name);
		if(!type)
			return;
		type->alphaFrom = from;
		type->alphaTo = to;
	}

	void Emitter::setScale(const std::string& name, float from, float to)
	{
		Type* type = findType(name);
		if(!type)
			return;
		type->scaleFrom = from;
		type->scaleTo = to;
	}

	void Emitter::setGravity(const std::string& name, float gravity)
	{
		Type* type = findType(name);
		if(type)
			type->gravity = gravity;
	}

	void Emitter::emit(const std::string& name, float x, float y)
	{
		const Type* type = findType(name);
		if(!type)
			return;

		float angle = type->angle + TF::random() * type->angleRange;
		float distance = type->distance + TF::random() * type->distanceRange;

		Particle particle;
		particle.type = type;
		particle.startX = x;
		particle.startY = y;
		particle.moveX = std::cos(angle) * distance;
		particle.moveY = std::sin(angle) * distance;
		particle.time = 0.0f;
		particle.duration = type->duration + TF::random() * type->durationRange;
		particles.push_back(particle);
	}

	void Emitter::update()
	{
		for(size_t i = 0; i < particles.size(); )
		{
			Particle& particle = particles[i];
			particle.time += TF::elapsed;
			if(particle.time >= particle.duration)
			{
				particle = particles.back();
				particles.pop_back();
				continue;
			}
			i++;
		}
	}

	void Emitter::render()
	{
		if(!texture)
			return;

		float originX = x - TF::camera->x * scrollX;
		float originY = y - TF::camera->y * scrollY;
		if(relative)
		{
			originX += point->x;
			originY += point->y;
		}

		for(size_t i = 0; i < particles.size(); i++)
		{
			const Particle& particle = particles[i];
			const Type& type = *particle.type;
			float t = progress(particle);
			float px, py;
			particlePosition((int)i, px, py);
			float scale = type.scaleFrom + (type.scaleTo - type.scaleFrom) * t;
			float particleAlpha = alpha * (type.alphaFrom + (type.alphaTo - type.alphaFrom) * t);

			size_t index = (size_t)(t * type.frames.size());
			if(index >= type.frames.size())
				index = type.frames.size() - 1;
			int frame = type.frames[index];

			// particles are centred on their point, so they shrink and grow in place
			float drawWidth = frameWidth * scale;
			float drawHeight = frameHeight * scale;
			Graphics::DrawRegionScaled(texture, (float)(frame % columns) * frameWidth, (float)(frame / columns) * frameHeight,
									   (float)frameWidth, (float)frameHeight,
									   originX + px - drawWidth * 0.5f, originY + py - drawHeight * 0.5f,
									   scale, scale, particleAlpha, false, 0.0f);
		}
	}

	float Emitter::progress(const Particle& particle)
	{
		return particle.duration > 0.0f ? particle.time / particle.duration : 1.0f;
	}

	void Emitter::particlePosition(int index, float& px, float& py) const
	{
		const Particle& particle = particles[index];
		float t = progress(particle);
		px = particle.startX + particle.moveX * t;
		py = particle.startY + particle.moveY * t + 0.5f * particle.type->gravity * particle.time * particle.time;
	}
}
