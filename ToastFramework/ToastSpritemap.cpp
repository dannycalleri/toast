/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastSpritemap.h"
#include "ToastGraphics.h"
#include "ToastTexture.h"
#include "ToastAnim.h"

#include <iostream>

namespace Toast
{
	Spritemap::Spritemap(const std::string& fileName, int frameWidth, int frameHeight) :
		Image(fileName),
		complete(true)
	{
		this->frameWidth=frameWidth;
		this->frameHeight=frameHeight;
		rect = new Rectangle(0.0f, 0.0f, frameWidth, frameHeight);
		if(frameWidth==0) rect->width = this->texture->getWidth();
		if(frameHeight==0) rect->height = this->texture->getHeight();

		this->columns = this->width / rect->width;
		this->rows = this->height / rect->height;
		this->frameCount = this->columns * this->rows;
		this->active=true;

		// Current animation is initialized to 0
		this->anim = 0;
		this->index = 0;
		this->frame = 0;

		this->timer = 0.0f;
	}

	Spritemap::~Spritemap()
	{
		// destroy all animations
		std::map<std::string, Anim*>::iterator i;

		for(i = anims.begin(); i != anims.end(); i++)
		{
			std::cout << "[SPRITEMAP destroy] deleting animation " << i->first << "\n";
			delete i->second;
		}

		delete rect;
	}

	/**
		Updating animation stuff
	*/
	void Spritemap::update()
	{

		if(anim && !complete)
		{
			timer += anim->frameRate * TF::elapsed;
			//std::cout << "TIMER BEFORE: " << timer << "\n";

			if(timer >= 1)
			{
				while (timer >= 1)
				{
					timer--;
					index++;

					//std::cout << "TIMER AFTER: " << timer << "\n";

					if (index == anim->frameCount)
					{
						if (anim->loop)
						{
							index = 0;
						}
						else
						{
							index = anim->frameCount - 1;
							complete = true;
							break;
						}
					}
				}

				if(anim) frame = anim->frames[index];

			}
		}

		updateSourceRect();
	}

	void Spritemap::render()
	{
		if(relative)
		{
			Graphics::DrawRegion(this->texture, rect->x, rect->y, rect->width, rect->height, this->point->x, this->point->y, this->alpha, this->flipped);
		}
		else
		{
			Graphics::DrawRegion(this->texture, rect->x, rect->y, rect->width, rect->height, this->x, this->y, this->alpha, this->flipped);
		}
	}

	void Spritemap::updateSourceRect()
	{
		rect->x = rect->width * (frame % columns);
		rect->y = rect->height * (unsigned int)(frame / columns);
	}

	void Spritemap::add(const std::string& name, int frames[],  unsigned int frameCount, unsigned int frameRate, bool loop)
	{
		Anim* tempAnim = new Anim(name, frames, frameCount, frameRate, loop);
		anims[name] = tempAnim;
	}

	void Spritemap::play(const std::string& name, bool reset, unsigned int frame)
	{
		// if the animation is already playing return
		if(!reset && this->anim && (this->anim->name == name)) return;

		this->anim = this->anims[name];

		// if animation doesn't exist
		if(!this->anim)
		{
			// resetting spritemap states
			this->frame = 0;
			this->index = 0;
			this->complete=true;

			// updating source rect for rendering
			updateSourceRect();
			
			return;
		}

		this->index = 0;
		this->complete = false;
		// update the frame and keep it inside the boundary of 
		// current animation frameCount
		this->frame = this->anim->frames[frame % this->anim->frameCount];
		updateSourceRect();

		return;
	}
}