/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTGRAPHIC_H
#define TOASTGRAPHIC_H

#include "ToastCommon.h"
#include "TF.h"

namespace Toast
{
	class Graphic
	{
	public:

		Graphic();
		virtual ~Graphic();

		virtual void update() {};

		/**
		 * Renders the graphic to the screen buffer.
		 * @param	point		The position to draw the graphic.
		 * @param	camera		The camera offset.
		 */
		virtual void render() {};


	public:
		bool active; // if the graphic should be updated
		bool visible; // if the graphic should be rendered
		float x;
		float y;
		int width;
		int height;
		float alpha;

		/**
		 * X scrollfactor, effects how much the camera offsets the drawn graphic.
		 * Can be used for parallax effect, eg. Set to 0 to follow the camera,
		 * 0.5 to move at half-speed of the camera, or 1 (default) to stay still.
		 */
		float scrollX;

		/**
		 * Y scrollfactor, effects how much the camera offsets the drawn graphic.
		 * Can be used for parallax effect, eg. Set to 0 to follow the camera,
		 * 0.5 to move at half-speed of the camera, or 1 (default) to stay still.
		 */
		float scrollY;

		/**
		 * If the graphic should render at its position relative to its parent Entity's position.
		 */
		bool relative;

		bool flipped;

	private:

		Point* point;
	};
}

#endif // TOASTGRAPHIC_H