/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "ToastConsole.h"
#include "ToastCanvas.h"
#include "ToastWorld.h"
#include "ToastEntity.h"
#include "ToastInput.h"

#include <iostream>
#include <cmath>

namespace Toast
{
	Console::Console()
	{
		this->canvas = 0;
		this->thickness=2;
		this->r=255;
		this->g=0;
		this->b=0;
		this->toggleKey = TOAST_KEY_ENTER;
		this->paused = false;
		this->enabled = false;
		this->selectionRect = new Rectangle();
		this->startDragPoint = new Point();
		this->startPoint = new Point();
		this->endPoint = new Point();
		this->selecting=false;
		this->selectedEntities.clear();
		this->selectedMouseOffsets.clear();
		this->dragging=false;
	}

	Console::~Console()
	{
		if(canvas)
			delete canvas;
		if(selectionRect)
			delete selectionRect;
		if(startPoint)
			delete startPoint;
		if(endPoint)
			delete endPoint;
		if(startDragPoint)
			delete startDragPoint;
	}

	void Console::setDrawColor(byte r, byte g, byte b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void Console::setThickness(float thickness)
	{
		this->thickness = thickness;
	}

	void Console::enable()
	{
		enabled = true;
		canvas = new Canvas(TF::width, TF::height);
		canvas->visible=false;
		canvas->alpha=0.4;
	}

	void Console::managePause()
	{
		TF::paused = paused;
		
		canvas->visible = paused;

		// activate/deactivate console panels
	}

	void Console::startSelection()
	{
		//clean up selected entities array
		selectedEntities.clear();
		selecting = true;

		startPoint->x = Input::mouseX;
		startPoint->y = Input::mouseY;
		
		selectionRect->x = Input::mouseX;
		selectionRect->y = Input::mouseY;

		std::cout << "START SELECTION x: " << selectionRect->x << " START SELECTION y: " << selectionRect->y << "\n";

		selectionRect->width = 0;
		selectionRect->height = 0;
	}

	void Console::updateSelection()
	{
		// updating selection ending point
		endPoint->x = Input::mouseX;
		endPoint->y = Input::mouseY;
		
		// updating selection rendering rect
		selectionRect->width = Input::mouseX - selectionRect->x;
		selectionRect->height = Input::mouseY - selectionRect->y;

		if (Input::mouseReleased)
		{
			selectEntities();
			selecting = false;
		}
	}

	void Console::selectEntities()
	{
		Entity* tmp = 0;
		for(size_t i=0;  i < TF::world->entities.size(); i++)
		{
			tmp = TF::world->entities[i];

			Rectangle tmpRect(startPoint->x + TF::camera->x, startPoint->y + TF::camera->y, endPoint->x - startPoint->x, endPoint->y - startPoint->y);
			
			Rectangle rect;
			rect.x = tmpRect.x;
			rect.y = tmpRect.y;
			rect.width = fabs(tmpRect.width);
			rect.height = fabs(tmpRect.height);
			if(tmpRect.width < 0) rect.x = endPoint->x;
			if(tmpRect.height < 0) rect.y = endPoint->y;
			
			if( (tmp->x >= rect.x && tmp->x <= (rect.x + rect.width)) &&
			    (tmp->y >= rect.y && tmp->y <= (rect.y + rect.height)) )
			{
				selectedEntities.push_back(tmp);
			}
			
		}
	}

	void Console::startDragEntities()
	{
		startDragPoint->x = Input::mouseX;
		startDragPoint->y = Input::mouseY;
		dragging=true;

		Entity* tmp = 0;
		// clean vector
		selectedMouseOffsets.clear();
		for(size_t i=0; i < selectedEntities.size(); i++)
		{
			tmp = selectedEntities[i];

			Point point;
			point.x = Input::mouseX - tmp->x;
			point.y = Input::mouseY - tmp->y;
			selectedMouseOffsets.push_back(point);
		}
	}

	void Console::moveEntities()
	{
		Entity* tmp = 0;
		for(size_t i=0; i < selectedEntities.size(); i++)
		{
			tmp = selectedEntities[i];

			Point point = selectedMouseOffsets[i];

			tmp->x = Input::mouseX - point.x;
			tmp->y = Input::mouseY - point.y;
			tmp->update();
		}

		if (Input::mouseReleased)
		{
			dragging = false;
		}
	}

	void Console::update()
	{
		if(!enabled) return;

		//std::cout << "MOUSE x: " << Input::mouseX*(TF::width/1024.0f) << " MOUSE y: " << Input::mouseY*(TF::height/768.0f) << "\n";
		//std::cout << "SCALE MOUSE x: " << Input::mouseScaleX << " SCALE MOUSE y: " << Input::mouseScaleY << "\n";


		// if console is in pause state
		if(paused)
		{
			// if the engine has been paused
			if(TF::paused)
			{
				if(Input::leftMousePressed)
				{
					if(Input::down(TOAST_KEY_LSHIFT) || Input::down(TOAST_KEY_RSHIFT))
					{
						// TODO: implement entities drag
						startDragEntities();
					}
					else
					{
						startSelection();
					}
				}
				else
				{
					if(selecting) updateSelection();
					if(dragging) moveEntities();
				}
			}
		}

		if(Input::pressed(toggleKey))
		{
			paused = !paused;
			managePause();
		}
	}

	void Console::render()
	{
		if(enabled && TF::world)
		{
			canvas->clear();

			Rectangle rect(0,0,canvas->width,canvas->height);
			canvas->fill(&rect, 0, 0, 0);

			// loop through all the entities and
			// render their hitboxes
			Entity* tmp=0;
			for(size_t i=0; i < TF::world->entities.size(); i++)
			{
				tmp = TF::world->entities[i];

				Rectangle rect(tmp->x - TF::camera->x, tmp->y - TF::camera->y, tmp->width, tmp->height);
				canvas->drawRect(&rect, this->r, this->g, this->b, this->thickness);
			}

			// debug
			//std::cout << "SELECTED ENTITIES : " << selectedEntities.size() << "\n";
			// DRAWING SELECTED ENTITIES
			tmp = 0;
			for(size_t i=0; i < selectedEntities.size(); i++)
			{
				tmp = selectedEntities[i];

				Rectangle rect(tmp->x - TF::camera->x, tmp->y - TF::camera->y, tmp->width, tmp->height);
				canvas->drawRect(&rect, 255, 255, 0, this->thickness);
			}

			if(selecting) 
			{
				canvas->drawRect(selectionRect, 255, 255, 0, this->thickness);
			}

			canvas->x = TF::camera->x;
			canvas->y = TF::camera->y;

			if(canvas->visible)
				canvas->render();
		}
	}
}