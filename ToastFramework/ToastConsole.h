/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

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