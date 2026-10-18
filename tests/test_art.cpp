#include "ToastTest.h"

#include "PixelArt.h"

#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace
{
	const std::string ART_DIR = std::string(TOAST_SOURCE_DIR) + "/ROGUELIKE/art/";
	const std::string ASSETS_DIR = std::string(TOAST_SOURCE_DIR) + "/ROGUELIKE/assets/";

	bool samePixels(SDL_Surface* a, SDL_Surface* b)
	{
		if(!a || !b || a->w != b->w || a->h != b->h)
			return false;
		for(int y = 0; y < a->h; y++)
		{
			for(int x = 0; x < a->w; x++)
			{
				Uint8 r1, g1, b1, a1, r2, g2, b2, a2;
				SDL_ReadSurfacePixel(a, x, y, &r1, &g1, &b1, &a1);
				SDL_ReadSurfacePixel(b, x, y, &r2, &g2, &b2, &a2);
				// fully transparent pixels may keep any colour in the PNG
				if(a1 != a2 || (a1 != 0 && (r1 != r2 || g1 != g2 || b1 != b2)))
					return false;
			}
		}
		return true;
	}

	std::string writeTemp(const std::string& name, const std::string& text)
	{
		std::string path = (std::filesystem::temp_directory_path() / name).string();
		std::ofstream(path.c_str()) << text;
		return path;
	}

	SDL_Surface* loadRGBA(const std::string& path)
	{
		SDL_Surface* loaded = IMG_Load(path.c_str());
		SDL_Surface* converted = loaded ? SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA32) : NULL;
		SDL_DestroySurface(loaded);
		return converted;
	}

	PixelArt::Palette loadPalette()
	{
		PixelArt::Palette palette;
		std::string error;
		PixelArt::loadPalette(ART_DIR + "palette.txt", palette, error);
		return palette;
	}
}

TOAST_TEST(art_committed_sheets_match_their_sources)
{
	PixelArt::Palette palette = loadPalette();
	CHECK(palette.colours.size() > 0 && palette.colours.size() <= 16);

	std::filesystem::path outDir = std::filesystem::temp_directory_path() / "toast-art-check";
	std::filesystem::create_directories(outDir);

	int sheets = 0;
	for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(ART_DIR))
	{
		if(entry.path().extension() != ".txt" || entry.path().filename() == "palette.txt")
			continue;

		PixelArt::Sheet sheet;
		std::string error;
		CHECK(PixelArt::loadSheet(entry.path().string(), palette, sheet, error));
		CHECK(PixelArt::writeSheet(sheet, palette, outDir.string(), error));

		std::vector<std::string> names(1, sheet.name);
		for(size_t v = 0; v < sheet.variants.size(); v++)
			names.push_back(sheet.variants[v].name);
		for(size_t n = 0; n < names.size(); n++)
		{
			SDL_Surface* fresh = loadRGBA((outDir / (names[n] + ".png")).string());
			SDL_Surface* committed = loadRGBA(ASSETS_DIR + names[n] + ".png");
			bool same = samePixels(fresh, committed);
			if(!same)
				SDL_Log("%s.png differs from its source; run: cmake --build build --target roguelike_art", names[n].c_str());
			SDL_DestroySurface(fresh);
			SDL_DestroySurface(committed);
			CHECK(same);
		}
		sheets++;
	}
	std::filesystem::remove_all(outDir);
	CHECK(sheets >= 2);
}

TOAST_TEST(art_unknown_colour_key_is_reported_with_its_line)
{
	PixelArt::Palette palette = loadPalette();
	std::string path = writeTemp("toast-bad-art.txt", "size 2 2\nframe one\nkk\nkZ\n");

	PixelArt::Sheet sheet;
	std::string error;
	CHECK(!PixelArt::loadSheet(path, palette, sheet, error));
	CHECK(error.find(path + ":4: unknown colour key 'Z'") == 0);
	std::filesystem::remove(path);
}

TOAST_TEST(art_flash_frames_are_white_copies)
{
	PixelArt::Palette palette = loadPalette();
	std::string path = writeTemp("toast-flash-art.txt",
		"size 2 1\ncolumns 8\nflash\n"
		"frame a\nk.\nframe b\n.r\nframe c\nkr\nframe d\nyy\n");

	PixelArt::Sheet sheet;
	std::string error;
	CHECK(PixelArt::loadSheet(path, palette, sheet, error));
	SDL_Surface* surface = PixelArt::render(sheet, palette);
	CHECK(surface && surface->w == 8 * 2 && surface->h == 1);

	for(int frame = 0; frame < 4; frame++)
	{
		for(int x = 0; x < 2; x++)
		{
			Uint8 r, g, b, a, fr, fg, fb, fa;
			SDL_ReadSurfacePixel(surface, frame * 2 + x, 0, &r, &g, &b, &a);
			SDL_ReadSurfacePixel(surface, (frame + 4) * 2 + x, 0, &fr, &fg, &fb, &fa);
			CHECK(fa == a);
			if(a)
				CHECK(fr == 255 && fg == 255 && fb == 255);
		}
	}
	SDL_DestroySurface(surface);
	std::filesystem::remove(path);
}

TOAST_TEST(art_palette_rejects_bad_colours_and_repeated_keys)
{
	const char* bad[] = {"k #1a1c2g\n", "k #zzzzzz\n", "k #1a1c2c\nk #ffffff\n", "k 1a1c2c\n"};
	for(size_t i = 0; i < sizeof(bad) / sizeof(bad[0]); i++)
	{
		std::string path = writeTemp("toast-bad-palette.txt", bad[i]);
		PixelArt::Palette palette;
		std::string error;
		CHECK(!PixelArt::loadPalette(path, palette, error));
		CHECK(error.find(path + ":") == 0);
		std::filesystem::remove(path);
	}
}

TOAST_TEST(art_variant_redraws_the_sheet_with_swapped_keys)
{
	PixelArt::Palette palette = loadPalette();
	std::string path = writeTemp("toast-variant-art.txt",
		"size 2 1\nflash\nvariant grey o=d\nframe a\nok\n");

	PixelArt::Sheet sheet;
	std::string error;
	CHECK(PixelArt::loadSheet(path, palette, sheet, error));
	CHECK(sheet.variants.size() == 1 && sheet.variants[0].name == "grey");

	SDL_Surface* swapped = PixelArt::render(sheet, palette, sheet.variants[0].swaps);
	Uint8 r, g, b, a;
	SDL_ReadSurfacePixel(swapped, 0, 0, &r, &g, &b, &a);
	SDL_Color grey = palette.colours['d'];
	CHECK(r == grey.r && g == grey.g && b == grey.b);
	// keys that are not swapped keep their colour, and flash frames stay white
	SDL_ReadSurfacePixel(swapped, 1, 0, &r, &g, &b, &a);
	CHECK(r == palette.colours['k'].r && g == palette.colours['k'].g);
	SDL_ReadSurfacePixel(swapped, 2, 0, &r, &g, &b, &a);
	CHECK(r == 255 && g == 255 && b == 255);
	SDL_DestroySurface(swapped);

	std::string bad = writeTemp("toast-bad-variant.txt", "size 1 1\nvariant grey o=Z\nframe a\no\n");
	CHECK(!PixelArt::loadSheet(bad, palette, sheet, error));
	CHECK(error.find(bad + ":2: bad swap") == 0);
	std::filesystem::remove(path);
	std::filesystem::remove(bad);
}
