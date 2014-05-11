/*
=========================================================================

Roguelike Dungeon Generation demo
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "Dungeon.h"
#include "GameWorld.h"
#include "RoomsManager.h"
#include "Room.h"
#include "TF.h"
#include "Game.h"
#include "ToastGrid.h"
#include "ToastTilemap.h"

#include <iostream>
#include <algorithm>
#include <functional>

namespace Toast
{
	Dungeon::Dungeon(GameWorld* gameWorld):
		world(gameWorld),
		MAX_TRIES(10)
	{
		size = 0;
		rooms.clear();
		tries = 0;
		cycles = 0;
		maxCycles = 0;
		startRoom = 0;
	}

	Dungeon::~Dungeon()
	{
		// delete memory here

		/*for(unsigned int i = 0; i < rooms.size(); i++)
		{
			TOAST_SAFE_DELETE(rooms[i]);
		}*/
	}

	void Dungeon::destroy()
	{
		// remove all added rooms
		world->removeAll();
		rooms.clear();
	}

	void Dungeon::buildRoom(Room* r)
	{
		world->roomsManager->addRoom(r);
		rooms.push_back(r);
	}

	void Dungeon::generate(int size)
	{
		// creating rooms for the 
		// current dungeon
		createRooms(size);
			
		// calculate distance
		// from the startRoom to
		// all other rooms
		calculateDistance();
			
		// find boss room
		findBossSpot();
			
		// closing opened doors
		closeDoors();
			
		// load rooms content
		loadRooms();

		std::cout << "[DUNGEON] generated " << size << " rooms dungeon\n";
	}

	void Dungeon::createRooms(unsigned int size)
	{
		this->size = size;
		cycles = 0;
		maxCycles = size * 10;

		//==================================
		// clean rooms vector
		/*for(int i=0; i < rooms.size(); i++)
		{
			Room* room = rooms[i];
			delete room;
		}
		*/

		rooms.clear();
		//==================================

		//qqq
		//world->roomsManager->loadStart();
		//rooms.push_back(world->roomsManager->startRoom);

		startRoom = world->roomsManager->loadStart();
		rooms.push_back(startRoom);

		bool roomAdded = false;
		Room* currentRoom = 0;
		unsigned int count = 0;
		unsigned int index = 0;

		while(rooms.size() < size - 1)
		{
			cycles++;

			if(cycles >= maxCycles)
			{
				// increase number of potential blocks
				destroy();
				createRooms(size);
				return;
			}

			currentRoom = rooms[count];

			if(currentRoom)
			{
				if(roomAdded && (count < rooms.size() - 1))
					count++;

				roomAdded = false;

				// randomly sort rooms vector
				std::random_shuffle(currentRoom->doors.begin(), currentRoom->doors.end());

				for(unsigned int i=0; i < currentRoom->doors.size(); i++)
				{
					Door* tempDoor = currentRoom->doors[i];

					if(tempDoor->opened)
					{
						if(tempDoor->type == DoorType::UP)
						{
							Room* downRoom = world->roomsManager->getRoom(DoorType::DOWN);
								
							if (downRoom)
							{
								if (downRoom->doors.size() < 2) // && count < size/2)
									break;
									
								Door* downDoor = downRoom->getDoor(DoorType::DOWN);
								float offsetX = tempDoor->x - downDoor->x;
								downRoom->x = currentRoom->x + offsetX;
								downRoom->y = currentRoom->y - downRoom->roomHeight;
									
								// add the room if it doesn't overlap
								// any other
								if (!downRoom->isOverlapping())
								{
									tries = 0;
									roomAdded = true;
									// door is now busy
									tempDoor->opened = false;
									downDoor->opened = false;
									// show room and save
									
									buildRoom(downRoom);
									
								}
								else
								{
									// cleanup
									world->roomsManager->destroyRoom(downRoom);
									downRoom = NULL;
										
									tries++;
									if (tries >= MAX_TRIES)
									{
										tries = 0;
										if (count < rooms.size() - 1)
										{
											count++;
										}
										else
										{
											//tries = 0;
											destroy();
											createRooms(size);
											return;
										}
									}
								}
							}
						}
						else if(tempDoor->type == DoorType::DOWN)
						{
							Room* upRoom = world->roomsManager->getRoom(DoorType::UP);
								
							if (upRoom)
							{
								if (upRoom->doors.size() < 2)// && count < size/2)
									break;
									
								Door* upDoor = upRoom->getDoor(DoorType::UP);
								float offsetX = tempDoor->x - upDoor->x;
								upRoom->x = currentRoom->x + offsetX;
								upRoom->y = currentRoom->y + currentRoom->roomHeight;
									
								if (!upRoom->isOverlapping())
								{
									tries = 0;
									roomAdded = true;
									// door is now busy
									tempDoor->opened = false;
									upDoor->opened = false;
									// show room and save
									buildRoom(upRoom);
								}
								else
								{
									// cleanup
									world->roomsManager->destroyRoom(upRoom);
									upRoom = NULL;
										
									tries++;
									if (tries >= MAX_TRIES)
									{
										tries = 0;
										if (count < rooms.size() - 1)
										{
											count++;
										}
										else
										{
											//tries = 0;
											destroy();
											createRooms(size);
											return;
										}
									}
								}
							}
						}
						else if(tempDoor->type == DoorType::LEFT)
						{
							Room* rightRoom = world->roomsManager->getRoom(DoorType::RIGHT);
								
							if (rightRoom)
							{
								if (rightRoom->doors.size() < 2)// && count < size/2)
									break;
									
								Door* rightDoor = rightRoom->getDoor(DoorType::RIGHT);
								float offsetY = tempDoor->y - rightDoor->y;
								rightRoom->x = currentRoom->x - rightRoom->roomWidth;
								rightRoom->y = currentRoom->y + offsetY;
									
								if (!rightRoom->isOverlapping())
								{
									tries = 0;
									roomAdded = true;
									// door is now busy
									tempDoor->opened = false;
									rightDoor->opened = false;
									// show room and save
									buildRoom(rightRoom);
								}
								else
								{
									// cleanup
									world->roomsManager->destroyRoom(rightRoom);
									rightRoom = NULL;
										
									tries++;
									if (tries >= MAX_TRIES)
									{
										tries = 0;
										if (count < rooms.size() - 1)
										{
											count++;
										}
										else
										{
											//tries = 0;
											destroy();
											createRooms(size);
											return;
										}
									}
								}
							}
						}
						else if(tempDoor->type == DoorType::RIGHT)
						{
							Room* leftRoom = world->roomsManager->getRoom(DoorType::LEFT);
								
							if (leftRoom)
							{
								if (leftRoom->doors.size() < 2)// && count < size/2)
									break;
									
								Door* leftDoor = leftRoom->getDoor(DoorType::LEFT);
								float offsetY = tempDoor->y - leftDoor->y;
								leftRoom->x = currentRoom->x + currentRoom->roomWidth;
								leftRoom->y = currentRoom->y + offsetY;
									
								if (!leftRoom->isOverlapping())
								{
									tries = 0;
									roomAdded = true;
									// door is now busy
									tempDoor->opened = false;
									leftDoor->opened = false;
									// show room and save
									buildRoom(leftRoom);
								}
								else
								{
									// cleanup
									world->roomsManager->destroyRoom(leftRoom);
									leftRoom = NULL;
										
									tries++;
									if (tries >= MAX_TRIES)
									{
										tries = 0;
										if (count < rooms.size() - 1)
										{
											count++;
										}
										else
										{
											//tries = 0;
											destroy();
											createRooms(size);
											return;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void Dungeon::calculateDistance()
	{
		for(unsigned int i=0; i < rooms.size(); i++)
		{
			Room* r = rooms[i];
			r->distance = TF::distance(startRoom->x, startRoom->y, r->x, r->y);
		}

		/*std::cout << "[DUNGEON] DISTANCE BEFORE ###" << "\n";
		for(unsigned int i = 0; i < rooms.size(); i++)
		{
			Room* r = rooms[i];
			std::cout << "[DUNGEON] ROOM DISTANCE = " << r->distance << "\n";
		}*/

		std::sort(rooms.begin(), rooms.end(), distanceCompare);

		std::cout << "\n[DUNGEON] DISTANCE AFTER SORTING ### " << "\n";
		for(unsigned int i = 0; i < rooms.size(); i++)
		{
			Room* r = rooms[i];
			std::cout << "[DUNGEON] ROOM DISTANCE = " << r->distance << "\n";
		}
	}

	bool Dungeon::distanceCompare(const Room* a, const Room* b)
	{
		if (b->distance < a->distance) return true;
		else if (b->distance >= a->distance) return false;

		return true;
	}
	
	void Dungeon::findBossSpot()
	{
		Room* bossRoom = world->roomsManager->loadBoss();
		bool added = false;

		Room* r = 0;
		for(unsigned int i=0; i < rooms.size(); i++)
		{
			r = rooms[i];
			
			Door* downDoor = bossRoom->getDoor(DoorType::DOWN);
				
			float offsetX = 0.0f;
			int numCols = r->roomWidth / Game::TILESIZE;
			int startCol = r->colStart + numCols / 2;
			bool demolition = false;
				
			if (!r->hasDoor(DoorType::UP))
			{
				offsetX = (startCol * Game::TILESIZE) - (downDoor->x + bossRoom->colStart * Game::TILESIZE);
				demolition=true;
			}
			else
			{
				offsetX = (r->colStart * Game::TILESIZE + r->getDoor(DoorType::UP)->x) - (downDoor->x + bossRoom->colStart * Game::TILESIZE);
			}
			
			bossRoom->x = r->x + offsetX;
			bossRoom->y = r->y - bossRoom->roomHeight;

			// room spot found
			if (!bossRoom->isOverlapping())
			{
				added=true;

				if(!demolition)
				{
					r->getDoor(DoorType::UP)->opened = false;
				}
				else
				{
					// demolition \m/
					createBossDoor(r, startCol);
				}

				downDoor->opened = false;
				// show room and save
				buildRoom(bossRoom);
					
				break;
			}
			/*else
			{
				TOAST_SAFE_DELETE(bossRoom);

				break;
			}*/
		}

		if(!added)
		{
			TOAST_SAFE_DELETE(bossRoom);
		}
	}

	void Dungeon::createBossDoor(Room* r, int startCol)
	{
		r->tilemap->setTile(startCol, r->rowStart, 1);
		r->tilemap->setTile(startCol+1, r->rowStart, 1);
		r->tilemap->setTile(startCol + 2, r->rowStart, 1);
			
		r->tilemap->setTile(startCol, r->rowStart+1, 1);
		r->tilemap->setTile(startCol+1, r->rowStart+1, 1);
		r->tilemap->setTile(startCol + 2, r->rowStart+1, 1);
			
		r->tilemap->setTile(startCol, r->rowStart+2, 1);
		r->tilemap->setTile(startCol+1, r->rowStart+2, 1);
		r->tilemap->setTile(startCol + 2, r->rowStart + 2, 1);
			
		r->collisionGrid->setTile(startCol, r->rowStart, false);
		r->collisionGrid->setTile(startCol+1, r->rowStart, false);
		r->collisionGrid->setTile(startCol + 2, r->rowStart, false);
			
		r->collisionGrid->setTile(startCol, r->rowStart+1, false);
		r->collisionGrid->setTile(startCol+1, r->rowStart+1, false);
		r->collisionGrid->setTile(startCol + 2, r->rowStart+1, false);
			
		r->collisionGrid->setTile(startCol, r->rowStart+2, false);
		r->collisionGrid->setTile(startCol+1, r->rowStart+2, false);
		r->collisionGrid->setTile(startCol + 2, r->rowStart+2, false);
			
		r->tilemap->setTile(startCol-1, r->rowStart, Room::DOOR_LEFT_TOP_TILE1);
		r->tilemap->setTile(startCol-1, r->rowStart+1, Room::DOOR_LEFT_TOP_TILE2);
		r->tilemap->setTile(startCol-1, r->rowStart+2, Room::DOOR_LEFT_TOP_TILE3);
			
		r->tilemap->setTile(startCol+3, r->rowStart, Room::DOOR_RIGHT_TOP_TILE1);
		r->tilemap->setTile(startCol+3, r->rowStart+1, Room::DOOR_RIGHT_TOP_TILE2);
		r->tilemap->setTile(startCol+3, r->rowStart+2, Room::DOOR_RIGHT_TOP_TILE3);
	}

	
	void Dungeon::closeDoors()
	{
		Room* temp = 0;

		for (unsigned int i = 0; i < rooms.size(); i++)
		{
			temp = rooms[i];
			
			Door* door = 0;
			for (unsigned int j = 0; j < temp->doors.size(); j++)
			{
				door = temp->doors[j];

				if (door->opened)
				{
					door->opened = false;
					int col = (door->x + temp->box->x) / Game::TILESIZE;
					int row = (door->y + temp->box->y) / Game::TILESIZE;
						
					if (door->type == DoorType::UP)
					{		
						// first col
						temp->tilemap->setTile(col, row, Room::WALL_TOP_TILE1);
						temp->tilemap->setTile(col, row+1, Room::WALL_TOP_TILE2);
						temp->tilemap->setTile(col, row + 2, Room::WALL_TOP_TILE3);
							
						// second col
						temp->tilemap->setTile(col + 1, row, Room::WALL_TOP_TILE1);
						temp->tilemap->setTile(col + 1, row + 1, Room::WALL_TOP_TILE2);
						temp->tilemap->setTile(col + 1, row + 2, Room::WALL_TOP_TILE3);
							
						// third col
						temp->tilemap->setTile(col + 2, row, Room::WALL_TOP_TILE1);
						temp->tilemap->setTile(col + 2, row + 1, Room::WALL_TOP_TILE2);
						temp->tilemap->setTile(col + 2, row + 2, Room::WALL_TOP_TILE3);
							
						// two more columns to be filled (pillars) in the first row
						temp->tilemap->setTile(col-1, row, Room::WALL_TOP_TILE1);
						temp->tilemap->setTile(col + 3, row, Room::WALL_TOP_TILE1);
							
						// ==============================
						// updating collisions layer
						temp->collisionGrid->setTile(col, row, true);
						temp->collisionGrid->setTile(col, row+1, true);
						temp->collisionGrid->setTile(col, row + 2, true);
							
						// second col
						temp->collisionGrid->setTile(col + 1, row, true);
						temp->collisionGrid->setTile(col + 1, row + 1, true);
						temp->collisionGrid->setTile(col + 1, row + 2, true);
							
						// third col
						temp->collisionGrid->setTile(col + 2, row, true);
						temp->collisionGrid->setTile(col + 2, row + 1, true);
						temp->collisionGrid->setTile(col + 2, row + 2, true);
							
						// two more columns to be filled (pillars) in the first row
						temp->collisionGrid->setTile(col-1, row, true);
						temp->collisionGrid->setTile(col + 3, row, true);
					}
					else if (door->type == DoorType::DOWN)
					{
						temp->tilemap->setTile(col-1, row, Room::WALL_DOWN_TILE);
						temp->tilemap->setTile(col, row, Room::WALL_DOWN_TILE);
						temp->tilemap->setTile(col+1, row, Room::WALL_DOWN_TILE);
						temp->tilemap->setTile(col+2, row, Room::WALL_DOWN_TILE);
						temp->tilemap->setTile(col + 3, row, Room::WALL_DOWN_TILE);
							
						// updating collisions
						temp->collisionGrid->setTile(col-1, row, true);
						temp->collisionGrid->setTile(col, row, true);
						temp->collisionGrid->setTile(col+1, row, true);
						temp->collisionGrid->setTile(col+2, row, true);
						temp->collisionGrid->setTile(col + 3, row, true);
					}
					else if (door->type == DoorType::LEFT)
					{
						temp->tilemap->setTile(col, row-2, Room::WALL_LEFT_TILE);
						temp->tilemap->setTile(col, row-1, Room::WALL_LEFT_TILE);
						temp->tilemap->setTile(col, row, Room::WALL_LEFT_TILE);
						temp->tilemap->setTile(col, row+1, Room::WALL_LEFT_TILE);
						temp->tilemap->setTile(col, row+2, Room::WALL_LEFT_TILE);
						temp->tilemap->setTile(col, row + 3, Room::WALL_LEFT_TILE);
							
						// updating collisions
						temp->collisionGrid->setTile(col, row-2, true);
						temp->collisionGrid->setTile(col, row-1, true);
						temp->collisionGrid->setTile(col, row, true);
						temp->collisionGrid->setTile(col, row+1, true);
						temp->collisionGrid->setTile(col, row+2, true);
						temp->collisionGrid->setTile(col, row+3, true);
					}
					else if (door->type == DoorType::RIGHT)
					{
							
						temp->tilemap->setTile(col, row-2, Room::WALL_RIGHT_TILE);
						temp->tilemap->setTile(col, row-1, Room::WALL_RIGHT_TILE);
						temp->tilemap->setTile(col, row, Room::WALL_RIGHT_TILE);
						temp->tilemap->setTile(col, row+1, Room::WALL_RIGHT_TILE);
						temp->tilemap->setTile(col, row+2, Room::WALL_RIGHT_TILE);
						temp->tilemap->setTile(col, row + 3, Room::WALL_RIGHT_TILE);
							
						temp->collisionGrid->setTile(col, row-2, true);
						temp->collisionGrid->setTile(col, row-1, true);
						temp->collisionGrid->setTile(col, row, true);
						temp->collisionGrid->setTile(col, row+1, true);
						temp->collisionGrid->setTile(col, row+2, true);
						temp->collisionGrid->setTile(col, row+3, true);
					}
				}
			}
		}
	}

	
	void Dungeon::loadRooms()
	{
	}
}