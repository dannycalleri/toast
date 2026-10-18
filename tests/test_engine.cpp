#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "ReadPixel.h"
#include "Toast.h"
#include "ToastCanvas.h"
#include "ToastFont.h"
#include "ToastGraphics.h"
#include "ToastImage.h"
#include "ToastInput.h"
#include "ToastText.h"
#include "ToastTexture.h"
#include "ToastWorld.h"

using namespace Toast;

namespace
{
	Engine* startEngine(unsigned int width, unsigned int height, bool gpu = false)
	{
		if(gpu)
		{
			SDL_ResetHint(SDL_HINT_VIDEO_DRIVER);
			SDL_ResetHint(SDL_HINT_RENDER_DRIVER);
		}
		else
		{
			useHeadlessVideo();
		}
		Engine* engine = new Engine(width, height);
		TF::world = new World();
		return engine;
	}

	SDL_Color pixelAt(int x, int y)
	{
		return readGamePixel(TF::engine->renderer, x, y);
	}

	void pushEvent(SDL_Event ev)
	{
		SDL_PushEvent(&ev);
		TF::engine->checkEvents();
	}
}

TOAST_TEST(engine_loads_assets_and_renders)
{
	Engine* engine = startEngine(320, 240);

	CHECK(TF::font && TF::font->getHandle());

	Image* image = new Image("controls.png");
	CHECK(image->texture != NULL);
	CHECK(image->width > 0 && image->height > 0);
	TF::world->addGraphic(image);

	Text* text = new Text("TOAST");
	CHECK(text->width > 0 && text->height > 0);
	TF::world->addGraphic(text);

	engine->resetRenderingBuffer();
	Graphics::Clear(0, 0, 0);
	TF::world->render();

	bool drewSomething = false;
	for(int x = 0; x < 320 && !drewSomething; x += 4)
		for(int y = 0; y < 240 && !drewSomething; y += 4)
		{
			SDL_Color c = pixelAt(x, y);
			drewSomething = c.r || c.g || c.b;
		}
	CHECK(drewSomething);

	delete engine;
}

namespace
{
	// freed by the TF texture cache when the Engine is deleted
	Texture* halfRedTexture()
	{
		SDL_Surface* surface = SDL_CreateSurface(16, 16, SDL_PIXELFORMAT_RGBA8888);
		SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGBA(surface, 255, 0, 0, 128));
		Texture* texture = new Texture();
		texture->tex = SDL_CreateTextureFromSurface(TF::engine->renderer, surface);
		texture->setWidth(16);
		texture->setHeight(16);
		SDL_DestroySurface(surface);
		TF::addTexture("half-red", texture);
		return texture;
	}

	SDL_Color drawHalfRedCanvasOverGrey(float canvasAlpha)
	{
		Canvas* canvas = new Canvas(16, 16);
		Rectangle src(0, 0, 16, 16);
		canvas->drawTextureRegion(halfRedTexture(), &src, 0, 0, false);
		canvas->alpha = canvasAlpha;
		TF::world->addGraphic(canvas);

		TF::engine->resetRenderingBuffer();
		Graphics::Clear(100, 100, 100);
		TF::world->render();
		return pixelAt(8, 8);
	}
}

TOAST_TEST(engine_translucent_canvas_content_blends_once)
{
	if(!gpuRendererAvailable()) SKIP("needs a GPU renderer");
	Engine* engine = startEngine(64, 64, true);

	// premultiplied: 128 + 100 * (1 - 0.5) = 178, 0 + 50 = 50.
	// Straight blending of the canvas would darken red to about 114.
	SDL_Color c = drawHalfRedCanvasOverGrey(1.0f);
	CHECK(c.r > 170 && c.r < 186);
	CHECK(c.g > 45 && c.g < 56);

	delete engine;
}

TOAST_TEST(engine_canvas_alpha_scales_color_and_coverage)
{
	if(!gpuRendererAvailable()) SKIP("needs a GPU renderer");
	Engine* engine = startEngine(64, 64, true);

	// canvas texel (128,0,0,128) at alpha 0.5: 64 + 100 * (1 - 0.25) = 139, 75
	SDL_Color c = drawHalfRedCanvasOverGrey(0.5f);
	CHECK(c.r > 131 && c.r < 147);
	CHECK(c.g > 69 && c.g < 81);

	delete engine;
}

TOAST_TEST(engine_text_is_drawn_inside_its_canvas)
{
	Engine* engine = startEngine(320, 240);

	// placed like DEMO's "PRESS [ENTER]" line, at the bottom of the screen
	Text* text = new Text("TOAST");
	text->y = TF::height - text->height;
	TF::world->addGraphic(text);

	engine->resetRenderingBuffer();
	Graphics::Clear(0, 0, 0);
	TF::world->render();

	bool drewText = false;
	for(int x = 0; x < text->width && !drewText; x++)
		for(int y = int(text->y); y < int(text->y) + text->height && !drewText; y++)
		{
			SDL_Color c = pixelAt(x, y);
			drewText = c.r || c.g || c.b;
		}
	CHECK(drewText);

	delete engine;
}

TOAST_TEST(engine_mouse_is_mapped_through_letterbox)
{
	Engine* engine = startEngine(320, 240);

	// twice as wide as the game: 160px bars left and right
	SDL_SetWindowSize(engine->window, 640, 240);
	SDL_SyncWindow(engine->window);
	engine->checkEvents();

	SDL_Event ev;
	SDL_zero(ev);
	ev.type = SDL_EVENT_MOUSE_MOTION;
	ev.motion.windowID = SDL_GetWindowID(engine->window);
	ev.motion.x = 170.0f;
	ev.motion.y = 20.0f;
	pushEvent(ev);

	CHECK(Input::mouseWindowX == 170);
	CHECK(Input::mouseX == 10);
	CHECK(Input::mouseY == 20);

	delete engine;
}

TOAST_TEST(engine_key_pressed_once_down_until_released)
{
	Engine* engine = startEngine(64, 64);

	SDL_Event ev;
	SDL_zero(ev);
	ev.type = SDL_EVENT_KEY_DOWN;
	ev.key.windowID = SDL_GetWindowID(engine->window);
	ev.key.scancode = SDL_SCANCODE_A;
	ev.key.down = true;
	pushEvent(ev);
	CHECK(Input::pressed(TOAST_KEY_A));
	CHECK(Input::down(TOAST_KEY_A));

	ev.key.repeat = true;
	pushEvent(ev);
	CHECK(!Input::pressed(TOAST_KEY_A));
	CHECK(Input::down(TOAST_KEY_A));

	ev.type = SDL_EVENT_KEY_UP;
	ev.key.repeat = false;
	ev.key.down = false;
	pushEvent(ev);
	CHECK(!Input::down(TOAST_KEY_A));

	delete engine;
}

TOAST_TEST(engine_frame_time_is_capped)
{
	const Uint64 ms = SDL_NS_PER_MS;
	CHECK(SDL_fabs(Engine::frameSeconds(1000 * ms, 1016 * ms) - 0.016) < 1e-9);
	CHECK(Engine::frameSeconds(0, 5000 * ms) == Engine::MAX_FRAME_SECONDS);
}

TOAST_TEST(engine_window_opens_at_the_largest_whole_scale_that_fits)
{
	int width, height;
	// 4x would need 960 of the 864 usable rows
	Engine::windowSize(320, 240, 1920, 1080, width, height);
	CHECK(width == 960 && height == 720);

	Engine::windowSize(320, 240, 2560, 1440, width, height);
	CHECK(width == 1280 && height == 960);

	Engine::windowSize(1280, 720, 1920, 1080, width, height);
	CHECK(width == 1280 && height == 720);

	// exactly 80%: the multiple still fits
	Engine::windowSize(320, 240, 800, 600, width, height);
	CHECK(width == 640 && height == 480);

	// smaller than 1x: shrink to 80% and keep the aspect ratio, as before
	Engine::windowSize(1280, 720, 1000, 1000, width, height);
	CHECK(width == 800 && height == 450);
}

namespace
{
	Image* whiteImage(int size)
	{
		SDL_Surface* surface = SDL_CreateSurface(size, size, SDL_PIXELFORMAT_RGBA8888);
		SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGBA(surface, 255, 255, 255, 255));
		Texture* texture = new Texture();
		texture->tex = SDL_CreateTextureFromSurface(TF::engine->renderer, surface);
		texture->setWidth(size);
		texture->setHeight(size);
		SDL_DestroySurface(surface);
		TF::addTexture("white", texture);

		Image* image = new Image("white");
		image->x = 10;
		image->y = 10;
		TF::world->addGraphic(image);
		return image;
	}

	void renderWorld()
	{
		TF::engine->resetRenderingBuffer();
		Graphics::Clear(0, 0, 0);
		TF::world->render();
	}
}

TOAST_TEST(engine_image_defaults_draw_at_full_size_and_opacity)
{
	Engine* engine = startEngine(64, 64);
	whiteImage(4);
	renderWorld();
	CHECK(pixelAt(10, 10).r == 255);
	CHECK(pixelAt(13, 13).r == 255);
	CHECK(pixelAt(14, 13).r == 0);
	CHECK(pixelAt(13, 14).r == 0);
	delete engine;
}

TOAST_TEST(engine_image_scale_multiplies_its_drawn_size)
{
	Engine* engine = startEngine(64, 64);
	Image* image = whiteImage(4);

	image->scale = 2;
	renderWorld();
	CHECK(pixelAt(17, 17).r == 255);
	CHECK(pixelAt(18, 17).r == 0);
	CHECK(pixelAt(17, 18).r == 0);

	image->scale = 1;
	image->scaleX = 2;
	renderWorld();
	CHECK(pixelAt(17, 13).r == 255);
	CHECK(pixelAt(18, 13).r == 0);
	CHECK(pixelAt(13, 14).r == 0);
	delete engine;
}

TOAST_TEST(engine_image_alpha_blends_over_what_is_below)
{
	Engine* engine = startEngine(64, 64);
	Image* image = whiteImage(4);
	image->alpha = 0.5f;
	renderWorld();
	SDL_Color c = pixelAt(11, 11);
	CHECK(c.r > 110 && c.r < 145);
	delete engine;
}
