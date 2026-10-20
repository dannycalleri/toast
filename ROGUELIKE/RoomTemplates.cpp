/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "RoomTemplates.h"
#include "TF.h"
#include "ToastError.h"
#include <fstream>

namespace Toast
{
	bool RoomTemplate::isSolid(int column, int row) const
	{
		return RoomTemplates::isSolid(at(column, row));
	}

	bool RoomTemplates::isKnown(char cell)
	{
		return cell == FLOOR || cell == WALL || cell == ROCK || cell == POT ||
			   cell == CRATE || cell == BONES || cell == GRASS || cell == SPAWN;
	}

	bool RoomTemplates::isSolid(char cell)
	{
		return cell == WALL || cell == ROCK || cell == POT || cell == CRATE;
	}

	void RoomTemplates::doorLane(Direction d, int& column, int& row)
	{
		switch(d)
		{
		case UP:    column = COLUMNS / 2 - 1; row = 0; break;
		case DOWN:  column = COLUMNS / 2 - 1; row = ROWS - 2; break;
		case LEFT:  column = 0; row = ROWS / 2 - 1; break;
		default:    column = COLUMNS - 2; row = ROWS / 2 - 1; break;
		}
	}

	bool RoomTemplates::load(const std::string& file)
	{
		std::ifstream in(TF::resolvePath(file).c_str());
		if(!in)
		{
			TOAST_ERROR(("[ROOMS] cannot open " + file).c_str());
			return false;
		}
		return parse(in, file);
	}

	bool RoomTemplates::parse(std::istream& in, const std::string& source)
	{
		RoomTemplate current;
		int headerLine = 0;
		bool broken = false;
		std::string line;

		for(int number = 1; ; number++)
		{
			bool more = (bool)std::getline(in, line);
			if(more && !line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);

			bool header = more && line.compare(0, 5, "room ") == 0;
			if(!more || header)
			{
				if(headerLine && !broken)
				{
					if((int)current.rows.size() == ROWS)
						templates.push_back(current);
					else
						TOAST_ERROR(("[ROOMS] " + source + ":" + std::to_string(headerLine) + ": room " + current.name +
									 " skipped, it has " + std::to_string(current.rows.size()) + " rows, not 10").c_str());
				}
				if(!more)
					break;
				current = RoomTemplate();
				current.name = line.substr(5);
				headerLine = number;
				broken = current.name.empty();
				if(broken)
					TOAST_ERROR(("[ROOMS] " + source + ":" + std::to_string(number) + ": room without a name skipped").c_str());
				continue;
			}

			if(line.empty() || line[0] == ';' || broken)
				continue;

			if(!headerLine)
			{
				TOAST_ERROR(("[ROOMS] " + source + ":" + std::to_string(number) + ": row before the first \"room <name>\" line").c_str());
				continue;
			}

			bool valid = (int)line.size() == COLUMNS && (int)current.rows.size() < ROWS;
			for(size_t i = 0; valid && i < line.size(); i++)
				valid = isKnown(line[i]);
			if(!valid)
			{
				TOAST_ERROR(("[ROOMS] " + source + ":" + std::to_string(number) + ": room " +
							 current.name + " skipped, bad row \"" + line + "\"").c_str());
				broken = true;
				continue;
			}
			current.rows.push_back(line);
		}
		return !templates.empty();
	}

	const RoomTemplate* RoomTemplates::find(const std::string& name) const
	{
		for(size_t i = 0; i < templates.size(); i++)
		{
			if(templates[i].name == name)
				return &templates[i];
		}
		return NULL;
	}

	std::vector<const RoomTemplate*> RoomTemplates::fighting() const
	{
		std::vector<const RoomTemplate*> result;
		for(size_t i = 0; i < templates.size(); i++)
		{
			if(templates[i].name != "start" && templates[i].name != "locked")
				result.push_back(&templates[i]);
		}
		return result;
	}
}
