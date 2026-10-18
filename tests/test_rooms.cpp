#include "ToastTest.h"

#include "RoomTemplates.h"
#include "TF.h"

#include <SDL3/SDL.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace Toast;

namespace
{
	std::vector<std::string> logged;
	SDL_LogOutputFunction previousLog;
	void* previousLogData;

	void captureLog(void*, int, SDL_LogPriority, const char* message)
	{
		logged.push_back(message);
	}

	void startCapture()
	{
		logged.clear();
		SDL_GetLogOutputFunction(&previousLog, &previousLogData);
		SDL_SetLogOutputFunction(captureLog, NULL);
	}

	void stopCapture()
	{
		SDL_SetLogOutputFunction(previousLog, previousLogData);
	}

	int reachableFrom(const RoomTemplate& room, int column, int row, std::vector<bool>& seen)
	{
		seen.assign(RoomTemplates::COLUMNS * RoomTemplates::ROWS, false);
		std::vector<int> stack(1, row * RoomTemplates::COLUMNS + column);
		seen[stack[0]] = true;
		int count = 0;
		while(!stack.empty())
		{
			int cell = stack.back();
			stack.pop_back();
			count++;
			int c = cell % RoomTemplates::COLUMNS;
			int r = cell / RoomTemplates::COLUMNS;
			const int dc[4] = {1, -1, 0, 0};
			const int dr[4] = {0, 0, 1, -1};
			for(int i = 0; i < 4; i++)
			{
				int nc = c + dc[i];
				int nr = r + dr[i];
				if(nc < 0 || nr < 0 || nc >= RoomTemplates::COLUMNS || nr >= RoomTemplates::ROWS)
					continue;
				int next = nr * RoomTemplates::COLUMNS + nc;
				if(!seen[next] && !room.isSolid(nc, nr))
				{
					seen[next] = true;
					stack.push_back(next);
				}
			}
		}
		return count;
	}
}

TOAST_TEST(rooms_every_template_parses)
{
	RoomTemplates rooms;
	startCapture();
	bool loaded = rooms.load("rooms/rooms.txt");
	stopCapture();
	CHECK(loaded);
	CHECK(logged.empty());

	std::ifstream file(TF::resolvePath("rooms/rooms.txt").c_str());
	std::string line;
	size_t headers = 0;
	while(std::getline(file, line))
		headers += line.compare(0, 5, "room ") == 0;
	CHECK(rooms.templates.size() == headers);

	CHECK(rooms.find("start") != NULL);
	CHECK(rooms.find("locked") != NULL);
	CHECK(rooms.fighting().size() >= 10);
}

TOAST_TEST(rooms_bad_template_is_skipped_and_logged)
{
	const std::string good(RoomTemplates::COLUMNS, '.');
	std::ostringstream text;
	text << "room fine\n";
	for(int r = 0; r < RoomTemplates::ROWS; r++)
		text << good << "\n";
	text << "room short_row\n" << good << "\n" << "....\n";
	for(int r = 2; r < RoomTemplates::ROWS; r++)
		text << good << "\n";
	text << "room odd_cell\n" << std::string(RoomTemplates::COLUMNS - 1, '.') << "X\n";
	for(int r = 1; r < RoomTemplates::ROWS; r++)
		text << good << "\n";
	text << "room too_few\n" << good << "\n";

	text << "room \n" << good << "\n";

	RoomTemplates rooms;
	std::istringstream in(".\n" + text.str());
	startCapture();
	bool parsed = rooms.parse(in, "test.txt");
	stopCapture();

	CHECK(parsed);
	CHECK(rooms.templates.size() == 1);
	CHECK(rooms.find("fine") != NULL);
	CHECK(logged.size() == 5);
	CHECK(logged[0].find("test.txt:1: row before") != std::string::npos);
	CHECK(logged[1].find("test.txt:15: room short_row") != std::string::npos);
	CHECK(logged[2].find("test.txt:25: room odd_cell") != std::string::npos);
	CHECK(logged[3].find("test.txt:35: room too_few") != std::string::npos);
	CHECK(logged[4].find("test.txt:37: room without a name") != std::string::npos);
}

TOAST_TEST(rooms_every_floor_cell_is_reachable_from_every_door)
{
	RoomTemplates rooms;
	CHECK(rooms.load("rooms/rooms.txt"));

	for(size_t i = 0; i < rooms.templates.size(); i++)
	{
		const RoomTemplate& room = rooms.templates[i];
		int walkable = 0;
		for(int r = 0; r < RoomTemplates::ROWS; r++)
			for(int c = 0; c < RoomTemplates::COLUMNS; c++)
				walkable += !room.isSolid(c, r);

		for(int d = UP; d <= RIGHT; d++)
		{
			int column, row;
			RoomTemplates::doorLane((Direction)d, column, row);
			for(int y = row; y < row + 2; y++)
				for(int x = column; x < column + 2; x++)
					CHECK(!room.isSolid(x, y));

			std::vector<bool> seen;
			bool reachesAll = reachableFrom(room, column, row, seen) == walkable;
			if(!reachesAll)
				SDL_Log("room %s: a floor cell cannot be reached from door %d", room.name.c_str(), d);
			CHECK(reachesAll);
		}
	}
}
