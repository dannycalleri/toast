/*
=========================================================================

Cat Crypt, a Toast sample game
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#include "PixelArt.h"
#include <cstdio>

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		std::fprintf(stderr, "usage: pixelart <output dir> <palette.txt> <sheet.txt>...\n");
		return 2;
	}

	std::string error;
	PixelArt::Palette palette;
	if(!PixelArt::loadPalette(argv[2], palette, error))
	{
		std::fprintf(stderr, "%s\n", error.c_str());
		return 1;
	}

	for(int i = 3; i < argc; i++)
	{
		PixelArt::Sheet sheet;
		if(!PixelArt::loadSheet(argv[i], palette, sheet, error) ||
		   !PixelArt::writeSheet(sheet, palette, argv[1], error))
		{
			std::fprintf(stderr, "%s\n", error.c_str());
			return 1;
		}
		std::printf("wrote %s\n", sheet.name.c_str());
	}
	return 0;
}
