/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef PIXELART_H
#define PIXELART_H

#include <SDL3/SDL.h>
#include <map>
#include <string>
#include <vector>

// palette.txt:  one colour per line, "<key> #rrggbb"; "." is always transparent.
// <sheet>.txt:
//   size <frameWidth> <frameHeight>
//   columns <n>                 frames per row of the sheet (default 8)
//   flash                       append an all-white copy of every frame
//   variant <name> a=b c=d ...  also write <name>.png with key a drawn as b, c as d
//   frame <label>               followed by frameHeight rows of frameWidth keys
// Lines starting with ';' are comments.
namespace PixelArt
{
	struct Palette
	{
		std::map<char, SDL_Color> colours;
	};

	struct Variant
	{
		std::string name;
		std::map<char, char> swaps;
	};

	struct Sheet
	{
		std::string name;
		int frameWidth;
		int frameHeight;
		int columns;
		bool flash;
		std::vector<std::vector<std::string>> frames;
		std::vector<Variant> variants;
	};

	// Each returns false and sets error to "<file>:<line>: <reason>" on bad input.
	bool loadPalette(const std::string& path, Palette& palette, std::string& error);
	bool loadSheet(const std::string& path, const Palette& palette, Sheet& sheet, std::string& error);

	// The caller frees the surface. swaps is empty for the sheet itself.
	SDL_Surface* render(const Sheet& sheet, const Palette& palette,
						const std::map<char, char>& swaps = std::map<char, char>());

	bool writeSheet(const Sheet& sheet, const Palette& palette, const std::string& outDir, std::string& error);
}

#endif // PIXELART_H
