/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef ROOM_H
#define ROOM_H

#include <ToastCommon.h>
#include <ToastEntity.h>
#include <vector>
#include "Door.h"

namespace Toast
{
	class Door;
	class GameWorld;

	class Room : public Entity
	{
	public:
		Room(char* filename, GameWorld* gameWorld);
		Room(const Room& rhs);
		virtual ~Room();

		// public methods
		virtual void update();

		bool hasDoor(DoorType type);
		Door* getDoor(DoorType type);
		bool isOverlapping();

	protected:
		void loadMap();
		void calculateBoundingBox();
		void locateDoors();
		bool overlapBoxVSBox(Rectangle* other, float otherX, float otherY);

	private:
		void stringCopy(char* dest, char* src);
		void stringLength(char* str);

	public:
		Tilemap* tilemap;
		int rowStart;
		int rowEnd;
		int colStart;
		int colEnd;
		int roomWidth;
		int roomHeight;
		std::vector<Door*> doors;
		float distance;
		Grid* collisionGrid;
		Rectangle* box;

		static const int WALL_RIGHT_TILE = 154;
		static const int WALL_LEFT_TILE = 152;
		static const int WALL_DOWN_TILE = 52;
		// 3 tiles for top wall
		static const int WALL_TOP_TILE1 = 49;
		static const int WALL_TOP_TILE2 = 50;
		static const int WALL_TOP_TILE3 = 66;
		
		static const int DOOR_LEFT_TOP_TILE1 = 72;
		static const int DOOR_LEFT_TOP_TILE2 = 51;
		static const int DOOR_LEFT_TOP_TILE3 = 67;
		
		static const int DOOR_RIGHT_TOP_TILE1 = 74;
		static const int DOOR_RIGHT_TOP_TILE2 = 51;
		static const int DOOR_RIGHT_TOP_TILE3 = 67;

	protected:
		GameWorld* world;
		char* filename;
		Grid* doorsGrid;
	};
}

#endif // ROOM_H