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

#ifndef TOASTCONSOLE_H
#define TOASTCONSOLE_H

#include "ToastCommon.h"
#include <vector>

namespace Toast
{
	class Console
	{
	public:
		Console();
		~Console();

		void enable();

		// modify drawing settings
		void setDrawColor(byte r, byte g, byte b);
		void setThickness(float thickness);

		void update();
		void render();

	private:
		void managePause();
		void startSelection();
		void updateSelection();
		void selectEntities();
		void startDragEntities();
		void moveEntities();

		Canvas* canvas;

		bool enabled;
		bool paused;
		bool selecting;
		bool dragging;

		// console toggle key
		unsigned int toggleKey;

		// start drag point
		Point* startDragPoint;

		// selection rect that will be rendered
		Rectangle* selectionRect;
		Point* startPoint;
		Point* endPoint;

		std::vector<Point> selectedMouseOffsets;
		std::vector<Entity*> selectedEntities;

		// parameters for customizing
		// the console rendering
		// default is 2
		float thickness;
		// default color is red
		byte r;
		byte g;
		byte b;
	};
}

#endif // TOASTCONSOLE_H