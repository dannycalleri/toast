#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "Toast.h"
#include "ToastTexture.h"
#include "ToastTilemap.h"
#include "ToastWorld.h"

using namespace Toast;

TOAST_TEST(tilemap_visible_range_covers_only_the_view)
{
	unsigned int first, end;

	Tilemap::visibleRange(0.0f, 100.0f, 32, 10, first, end);
	CHECK(first == 0 && end == 4);

	// scrolled 40px: tile 0 is off-screen, tile 4 is partly visible
	Tilemap::visibleRange(-40.0f, 100.0f, 32, 10, first, end);
	CHECK(first == 1 && end == 5);

	// map entirely right of the view, then entirely left of it
	Tilemap::visibleRange(500.0f, 100.0f, 32, 10, first, end);
	CHECK(first == end);
	Tilemap::visibleRange(-1000.0f, 100.0f, 32, 10, first, end);
	CHECK(first == end);
}

namespace
{
	// tile 0 white, tile 1 red, 16x16 each
	void addTwoTileTileset()
	{
		SDL_Surface* surface = SDL_CreateSurface(32, 16, SDL_PIXELFORMAT_RGBA8888);
		SDL_Rect left = {0, 0, 16, 16};
		SDL_Rect right = {16, 0, 16, 16};
		SDL_FillSurfaceRect(surface, &left, SDL_MapSurfaceRGBA(surface, 255, 255, 255, 255));
		SDL_FillSurfaceRect(surface, &right, SDL_MapSurfaceRGBA(surface, 255, 0, 0, 255));
		Texture* texture = new Texture();
		texture->tex = SDL_CreateTextureFromSurface(TF::engine->renderer, surface);
		texture->setWidth(32);
		texture->setHeight(16);
		SDL_DestroySurface(surface);
		TF::addTexture("two-tiles", texture);
	}

	SDL_Color renderAndReadPixel(int x, int y)
	{
		TF::engine->resetRenderingBuffer();
		Graphics::Clear(0, 0, 0);
		TF::world->render();

		SDL_Color color = {0, 0, 0, 0};
		SDL_Rect rect = {x, y, 1, 1};
		SDL_Surface* surface = SDL_RenderReadPixels(TF::engine->renderer, &rect);
		SDL_ReadSurfacePixel(surface, 0, 0, &color.r, &color.g, &color.b, &color.a);
		SDL_DestroySurface(surface);
		return color;
	}
}

TOAST_TEST(tilemap_draws_set_tiles_where_the_camera_shows_them)
{
	useHeadlessVideo();
	Engine* engine = new Engine(64, 16);
	TF::world = new World();
	addTwoTileTileset();

	Tilemap* tilemap = new Tilemap("two-tiles", 64, 16, 16, 16);
	tilemap->setTile(1, 0, 1);
	TF::world->addGraphic(tilemap);

	SDL_Color empty = renderAndReadPixel(8, 8);
	SDL_Color red = renderAndReadPixel(24, 8);
	CHECK(empty.r == 0 && empty.g == 0);
	CHECK(red.r == 255 && red.g == 0);

	TF::camera->x = 16;
	SDL_Color scrolled = renderAndReadPixel(8, 8);
	CHECK(scrolled.r == 255 && scrolled.g == 0);

	delete engine;
}

TOAST_TEST(tilemap_linear_filtering_does_not_bleed_the_next_atlas_tile)
{
	if(!gpuRendererAvailable()) SKIP("needs a GPU renderer");
	Engine* engine = new Engine(64, 64);
	TF::world = new World();

	// tile 0 white directly above tile 1 red, as DEMO's atlas stacks its tiles
	SDL_Surface* surface = SDL_CreateSurface(16, 32, SDL_PIXELFORMAT_RGBA8888);
	SDL_Rect top = {0, 0, 16, 16};
	SDL_Rect bottom = {0, 16, 16, 16};
	SDL_FillSurfaceRect(surface, &top, SDL_MapSurfaceRGBA(surface, 255, 255, 255, 255));
	SDL_FillSurfaceRect(surface, &bottom, SDL_MapSurfaceRGBA(surface, 255, 0, 0, 255));
	Texture* texture = new Texture();
	texture->tex = SDL_CreateTextureFromSurface(engine->renderer, surface);
	SDL_SetTextureScaleMode(texture->tex, SDL_SCALEMODE_LINEAR);
	texture->setWidth(16);
	texture->setHeight(32);
	SDL_DestroySurface(surface);
	TF::addTexture("stacked-tiles", texture);

	Tilemap* tilemap = new Tilemap("stacked-tiles", 64, 64, 16, 16);
	tilemap->setTile(0, 0, 0);
	TF::world->addGraphic(tilemap);

	// DEMO scales its logical size to a smaller window, and smooth
	// scrolling leaves the camera between pixels
	SDL_SetWindowSize(engine->window, 40, 40);
	SDL_SyncWindow(engine->window);
	engine->checkEvents();
	TF::camera->y = -0.3f;
	engine->resetRenderingBuffer();
	Graphics::Clear(0, 0, 0);
	TF::world->render();

	// one output-pixel column through the tile, down past its bottom edge where a bleed would show
	int outputW, outputH;
	SDL_GetCurrentRenderOutputSize(engine->renderer, &outputW, &outputH);
	SDL_SetRenderLogicalPresentation(engine->renderer, 0, 0, SDL_LOGICAL_PRESENTATION_DISABLED);
	SDL_Rect column = {outputW * 8 / 64, 0, 1, outputH};
	SDL_Surface* pixels = SDL_RenderReadPixels(engine->renderer, &column);

	CHECK(pixels != NULL);

	bool reddish = false;
	bool white = false;
	for(int y = 0; y < outputH; y++)
	{
		SDL_Color c = {0, 0, 0, 0};
		SDL_ReadSurfacePixel(pixels, 0, y, &c.r, &c.g, &c.b, &c.a);
		if(c.r > c.g + 40)
			reddish = true;
		if(c.r > 200 && c.g > 200)
			white = true;
	}
	SDL_DestroySurface(pixels);
	CHECK(white);
	CHECK(!reddish);

	delete engine;
}
