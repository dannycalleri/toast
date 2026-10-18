/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "PixelArt.h"
#include <SDL3_image/SDL_image.h>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>

namespace PixelArt
{
	namespace
	{
		std::string baseName(const std::string& path)
		{
			size_t slash = path.find_last_of("/\\");
			std::string name = slash == std::string::npos ? path : path.substr(slash + 1);
			size_t dot = name.find_last_of('.');
			return dot == std::string::npos ? name : name.substr(0, dot);
		}

		std::string where(const std::string& path, int line)
		{
			std::ostringstream out;
			out << path << ":" << line << ": ";
			return out.str();
		}

		void stripCarriageReturn(std::string& line)
		{
			if(!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
		}
	}

	bool loadPalette(const std::string& path, Palette& palette, std::string& error)
	{
		std::ifstream file(path.c_str());
		if(!file)
		{
			error = path + ": cannot open";
			return false;
		}

		std::string line;
		for(int number = 1; std::getline(file, line); number++)
		{
			stripCarriageReturn(line);
			if(line.empty() || line[0] == ';')
				continue;

			char key;
			std::string hex;
			std::istringstream in(line);
			if(!(in >> key >> hex) || key == '.' || hex.size() != 7 || hex[0] != '#')
			{
				error = where(path, number) + "expected \"<key> #rrggbb\"";
				return false;
			}
			bool digits = true;
			for(size_t i = 1; i < hex.size(); i++)
				digits = digits && std::isxdigit((unsigned char)hex[i]);
			if(!digits)
			{
				error = where(path, number) + "\"" + hex + "\" is not a #rrggbb colour";
				return false;
			}
			unsigned long rgb = std::strtoul(hex.c_str() + 1, NULL, 16);
			if(palette.colours.count(key))
			{
				error = where(path, number) + "key '" + key + "' is already in the palette";
				return false;
			}
			palette.colours[key] = SDL_Color{(Uint8)(rgb >> 16), (Uint8)(rgb >> 8), (Uint8)rgb, 255};
		}
		return true;
	}

	bool loadSheet(const std::string& path, const Palette& palette, Sheet& sheet, std::string& error)
	{
		std::ifstream file(path.c_str());
		if(!file)
		{
			error = path + ": cannot open";
			return false;
		}

		sheet = Sheet();
		sheet.name = baseName(path);
		sheet.frameWidth = 0;
		sheet.frameHeight = 0;
		sheet.columns = 8;
		sheet.flash = false;

		std::vector<std::string>* frame = NULL;
		std::string line;
		for(int number = 1; std::getline(file, line); number++)
		{
			stripCarriageReturn(line);
			if(frame && (int)frame->size() < sheet.frameHeight)
			{
				if((int)line.size() != sheet.frameWidth)
				{
					error = where(path, number) + "frame row must be " + std::to_string(sheet.frameWidth) + " keys wide";
					return false;
				}
				for(size_t i = 0; i < line.size(); i++)
				{
					if(line[i] != '.' && !palette.colours.count(line[i]))
					{
						error = where(path, number) + "unknown colour key '" + line[i] + "'";
						return false;
					}
				}
				frame->push_back(line);
				continue;
			}

			if(line.empty() || line[0] == ';')
				continue;

			std::istringstream in(line);
			std::string word;
			in >> word;
			if(word == "size")
				in >> sheet.frameWidth >> sheet.frameHeight;
			else if(word == "columns")
				in >> sheet.columns;
			else if(word == "flash")
				sheet.flash = true;
			else if(word == "frame")
			{
				if(sheet.frameWidth <= 0 || sheet.frameHeight <= 0 || sheet.columns <= 0)
				{
					error = where(path, number) + "\"size\" and \"columns\" must come before the first frame";
					return false;
				}
				sheet.frames.push_back(std::vector<std::string>());
				frame = &sheet.frames.back();
			}
			else
			{
				error = where(path, number) + "unknown directive \"" + word + "\"";
				return false;
			}
		}

		if(sheet.frames.empty() || (int)sheet.frames.back().size() != sheet.frameHeight)
		{
			error = path + ": the last frame is incomplete or there are no frames";
			return false;
		}
		return true;
	}

	SDL_Surface* render(const Sheet& sheet, const Palette& palette)
	{
		int count = (int)sheet.frames.size() * (sheet.flash ? 2 : 1);
		int columns = count < sheet.columns ? count : sheet.columns;
		int rows = (count + sheet.columns - 1) / sheet.columns;

		SDL_Surface* surface = SDL_CreateSurface(columns * sheet.frameWidth, rows * sheet.frameHeight, SDL_PIXELFORMAT_RGBA32);
		if(!surface)
			return NULL;
		SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0));

		for(int index = 0; index < count; index++)
		{
			const std::vector<std::string>& frame = sheet.frames[index % sheet.frames.size()];
			bool white = index >= (int)sheet.frames.size();
			int originX = (index % sheet.columns) * sheet.frameWidth;
			int originY = (index / sheet.columns) * sheet.frameHeight;

			for(int y = 0; y < sheet.frameHeight; y++)
			{
				for(int x = 0; x < sheet.frameWidth; x++)
				{
					char key = frame[y][x];
					if(key == '.')
						continue;
					SDL_Color c = white ? SDL_Color{255, 255, 255, 255} : palette.colours.find(key)->second;
					SDL_WriteSurfacePixel(surface, originX + x, originY + y, c.r, c.g, c.b, c.a);
				}
			}
		}
		return surface;
	}

	bool writeSheet(const Sheet& sheet, const Palette& palette, const std::string& outDir, std::string& error)
	{
		SDL_Surface* surface = render(sheet, palette);
		std::string path = outDir + "/" + sheet.name + ".png";
		bool saved = surface && IMG_SavePNG(surface, path.c_str());
		SDL_DestroySurface(surface);
		if(!saved)
		{
			error = path + ": " + SDL_GetError();
			return false;
		}
		return true;
	}
}
