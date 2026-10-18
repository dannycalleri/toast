/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Room.h"
#include "rapidxml.hpp"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include "TF.h"
#include "ToastGrid.h"
#include "ToastTilemap.h"
#include "Game.h"
#include "GameWorld.h"
#include "Dungeon.h"

using namespace rapidxml;

namespace Toast
{
	Room::Room(const std::string& filename, GameWorld* gameWorld) :
		Entity()
	{
		this->filename = filename;
		this->world = gameWorld;
		tilemap = 0;
		doorsGrid = 0;
		collisionGrid = 0;
		rowStart = 0;
		rowEnd = 0;
		colStart = 0;
		colEnd = 0;
		roomWidth = 0;
		roomHeight = 0;
		box = 0;
		distance=0.0f;

		doors.clear();

		loadMap();
		calculateBoundingBox();
		locateDoors();

		this->graphic = tilemap;
	}

	Room::Room(const Room& rhs)
	{
		this->filename = rhs.filename;
		this->world = rhs.world;
		// the dungeon copies rooms by the hundred while it tries to place them
		tilemap = rhs.tilemap ? new Tilemap(*rhs.tilemap) : 0;
		doorsGrid = rhs.doorsGrid ? new Grid(*rhs.doorsGrid) : 0;
		collisionGrid = rhs.collisionGrid ? new Grid(*rhs.collisionGrid) : 0;
		rowStart = 0;
		rowEnd = 0;
		colStart = 0;
		colEnd = 0;
		roomWidth = 0;
		roomHeight = 0;
		box = 0;
		distance=0.0f;

		doors.clear();

		calculateBoundingBox();
		locateDoors();

		for(unsigned int i=0; i < doors.size(); i++)
		{
			Door* door = doors[i];
			door->opened=true;
		}

		this->graphic = tilemap;
	}

	Room::~Room()
	{
		delete doorsGrid;
		delete collisionGrid;
		delete box;

		// delete every door
		for(unsigned int i=0; i < doors.size(); i++)
		{
			delete doors[i];
		}
		doors.clear();
	}

	bool Room::overlapBoxVSBox(Rectangle* other, float otherX, float otherY)
	{
		float x1 = this->x + this->box->x;
		float y1 = this->y + this->box->y;

		float x2 = otherX + other->x;
		float y2 = otherY + other->y;

		float px = fabs(x2 - x1) * 2;
		float py = fabs(y2 - y1) * 2;
			
		if (px < (this->box->width + other->width) && py < (this->box->height + other->height)) return true;
		return false;
	}

	bool Room::isOverlapping()
	{
		for(unsigned int i = 0; i < world->dungeon->rooms.size(); i++)
		{
			Room* r = (Room*)world->dungeon->rooms[i];
			
			if(r != this)
			{
				if (overlapBoxVSBox(r->box, r->x, r->y))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool Room::hasDoor(DoorType type)
	{
		for (unsigned int i = 0; i < doors.size(); i++)
		{
			Door* tempDoor = doors[i];
			if (tempDoor->type == type)
				return true;
		}
			
		return false;
	}

	Door* Room::getDoor(DoorType type)
	{
		for (unsigned int i = 0; i < doors.size(); i++)
		{
			Door* tempDoor = doors[i];
			if (tempDoor->type == type)
				return tempDoor;
		}
			
		return NULL;
	}

	void Room::locateDoors()
	{
		if(!doorsGrid)
			return;

		bool doorFound = false;
		
		// find left edge doors
		for(int row = rowStart; row <= rowEnd; row++)
		{
			if(doorsGrid->getTile(colStart, row))
			{
				if(!doorFound)
				{
					doorFound=true;

					Door* tempDoor = new Door(colStart * Game::TILESIZE - box->x, 
											  row * Game::TILESIZE - box->y, 
											  DoorType::LEFT);
					doors.push_back(tempDoor);
				}
			}
			else
			{
				if(doorFound)
				{
					doorFound = false;
				}
			}
		}

		doorFound=false;

		// right edge doors
		for (int row = rowStart; row <= rowEnd; row++)
		{
			if (doorsGrid->getTile(colEnd, row))
			{
				if (!doorFound)
				{
					doorFound = true;
					Door* tempDoor = new Door(colEnd * Game::TILESIZE - box->x, 
										row * Game::TILESIZE - box->y, 
										DoorType::RIGHT);
					doors.push_back(tempDoor);
				}
			}
			else
			{
				if (doorFound)
				{
					doorFound = false;
				}
			}
		}
			
		doorFound = false;

		// top edge doors
		for (int col = colStart; col <= colEnd; col++)
		{
			if (doorsGrid->getTile(col, rowStart))
			{
				if (!doorFound)
				{
					doorFound = true;
						
					Door* tempDoor = new Door(col * Game::TILESIZE - box->x, 
										rowStart * Game::TILESIZE - box->y, 
										DoorType::UP);
					doors.push_back(tempDoor);
				}
			}
			else
			{
				if (doorFound)
				{
					doorFound = false;
				}
			}
		}
			
		doorFound = false;

		// bottom edge doors
		for (int col = colStart; col <= colEnd; col++)
		{
			if (doorsGrid->getTile(col, rowEnd))
			{
				if (!doorFound)
				{
					doorFound = true;
					Door* tempDoor = new Door(col * Game::TILESIZE - box->x,
										rowEnd * Game::TILESIZE - box->y,
										DoorType::DOWN);
					doors.push_back(tempDoor);
				}
			}
			else
			{
				if (doorFound)
				{
					doorFound = false;
				}
			}
		}
	}

	void Room::calculateBoundingBox()
	{
		bool startFound=false;

		// a room whose file failed to load keeps an empty box
		for(int row = 0; collisionGrid && row < collisionGrid->rows; row++)
		{
			for(int col = 0; col < collisionGrid->columns; col++)
			{
				if(collisionGrid->getTile(col, row))
				{
					if(!startFound)
					{
						rowStart = row;
						colStart = col;
						startFound = true;
					}

					rowEnd = row;
					colEnd = col;
				}
			}
		}

		// make sure that the room is bigger than 1x1
		if (rowStart < rowEnd && colStart < colEnd)
		{
			int rowsSpan = rowEnd - rowStart;
			int colsSpan = colEnd - colStart;
				
			roomWidth = (colsSpan + 1) * Game::TILESIZE;
			roomHeight = (rowsSpan + 1) * Game::TILESIZE;
		}

		box = new Rectangle(
			colStart * Game::TILESIZE,
			rowStart * Game::TILESIZE,
			roomWidth,
			roomHeight);

		//std::cout << "box : " << "colStart = " << box->x << " rowStart = " << box->y << " roomWidth = " << box->width << " roomHeight = " << box->height << "\n";
	}

	void Room::loadMap()
	{
		std::ifstream mapFile(TF::resolvePath(filename).c_str(), std::ios::binary);

		if(mapFile.fail())
		{
			TOAST_ERROR(("[ROOM] failed to open room file " + filename).c_str());
			return;
		}

		// rapidxml parses in place and needs a writable, null-terminated buffer
		std::vector<char> mapText((std::istreambuf_iterator<char>(mapFile)),
								  std::istreambuf_iterator<char>());
		mapText.push_back('\0');

		xml_document<> map;
		try
		{
			map.parse<0>(&mapText[0]);
		}
		catch(const parse_error& e)
		{
			TOAST_ERROR(("[ROOM] " + filename + ": " + e.what()).c_str());
			return;
		}

		xml_node<>* level = map.first_node();
		xml_attribute<>* mapWidth = level ? level->first_attribute("width") : NULL;
		xml_attribute<>* mapHeight = level ? level->first_attribute("height") : NULL;
		xml_node<>* doorsNode = level ? level->first_node("Doors") : NULL;
		xml_node<>* tilesNode = level ? level->first_node("Tiles") : NULL;
		xml_node<>* collisionsNode = level ? level->first_node("Collisions") : NULL;
		if(!mapWidth || !mapHeight || !doorsNode || !tilesNode || !collisionsNode)
		{
			TOAST_ERROR(("[ROOM] " + filename + " is missing width, height, Doors, Tiles or Collisions").c_str());
			return;
		}

		doorsGrid = new Grid(atoi(mapWidth->value()), atoi(mapHeight->value()), 32, 32);
		doorsGrid->loadFromString(doorsNode->value(), "", "\n");
		//doorsGrid->printDebug();

		collisionGrid = new Grid(atoi(mapWidth->value()), atoi(mapHeight->value()), 32, 32);
		collisionGrid->loadFromString(collisionsNode->value(), "", "\n");
		//collisionGrid->printDebug();

		tilemap = new Tilemap("assets/tileset.png", atoi(mapWidth->value()), atoi(mapHeight->value()), 32, 32);
		tilemap->loadFromString(tilesNode->value(), ",", "\n");

		/*std::cout << "DOORS GRID WIDTH = " << doorsGrid->width << "\n";
		std::cout << "DOORS GRID HEIGHT = " << doorsGrid->height << "\n";*/
	}

	void Room::update()
	{
		Entity::update();
	}
}