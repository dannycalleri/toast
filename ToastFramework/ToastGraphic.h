/*
=========================================================================

Toast 2D game framework
Copyright (C)2012 Danny Calleri

Toast 2D game framework is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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