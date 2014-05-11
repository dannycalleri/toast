/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Room.h"
#include <rapidxml-1.13\rapidxml.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include "ToastGrid.h"
#include "ToastTilemap.h"
#include "Game.h"
#include "GameWorld.h"
#include "Dungeon.h"

using namespace rapidxml;

namespace Toast
{
	Room::Room(char* filename, GameWorld* gameWorld) :
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
		//int filenameLength = strlen(rhs.filename);
		//this->filename = new char[filenameLength];
		//this->filename = "";
		//stringCopy(this->filename, rhs.filename);
		this->world = rhs.world;
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

	void Room::stringCopy(char* dest, char* src)
	{
		// while pointer does not equal null char, copy contents
		while( *src != '\0' )
		{
			*dest = *src;
			src++;
			dest++;
		}

		// null terminating dest
		dest++;
		*dest = '\0';
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
		bool doorFound = false;
		int leftDoorsCount = 0;
		
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
					leftDoorsCount++;
				}
			}
		}

		if(doorFound)
		{
			leftDoorsCount++;
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

		for(int row = 0; row < collisionGrid->rows; row++)
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
		std::ifstream mapFile;
		mapFile.open(filename);

		if(mapFile.fail())
		{
			TOAST_ERROR("[ROOM] failed to open room file");
			return;
		}

		std::string mapString = "";
		mapFile.seekg(0, std::ios::end);   
		mapString.reserve(mapFile.tellg());
		mapFile.seekg(0, std::ios::beg);

		mapString.assign((std::istreambuf_iterator<char>(mapFile)),
					std::istreambuf_iterator<char>());

		//std::cout << "[XML MAP]: \n" << mapString << "\n";

		// closing file stream
		mapFile.close();

		xml_document<> map;
		map.parse<0>((char*)mapString.c_str());

		xml_attribute<>* mapWidth = map.first_node()->first_attribute();
		xml_attribute<>* mapHeight = mapWidth->next_attribute();
		xml_node<>* doorsNode = map.first_node()->first_node("Doors");
		xml_node<>* tilesNode = map.first_node()->first_node("Tiles");
		xml_node<>* collisionsNode = map.first_node()->first_node("Collisions");
		//std::cout << "Doors = " << doorsNode->value() << "\n";

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