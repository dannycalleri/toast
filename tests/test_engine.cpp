// Runs the real Engine on SDL's dummy video driver with the software renderer,
// so most of these tests need no display.

#include "ToastTest.h"
#include "Toast.h"
#include "ToastCanvas.h"
#include "ToastFont.h"
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
			SDL_SetHintWithPriority(SDL_HINT_VIDEO_DRIVER, "dummy", SDL_HINT_OVERRIDE);
			SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "software", SDL_HINT_OVERRIDE);
		}
		Engine* engine = new Engine(width, height);
		TF::world = new World();
		return engine;
	}

	SDL_Color pixelAt(int x, int y)
	{
		SDL_Color color = {0, 0, 0, 0};
		SDL_Rect rect = {x, y, 1, 1};
		SDL_Surface* surface = SDL_RenderReadPixels(TF::engine->renderer, &rect);
		if(surface)
		{
			SDL_ReadSurfacePixel(surface, 0, 0, &color.r, &color.g, &color.b, &color.a);
			SDL_DestroySurface(surface);
		}
		return color;
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

	// SDL's software renderer stores opaque alpha when blending into a
	// render target, so translucent canvas content can only be checked on a GPU.
	// Probed before the Engine starts, because the Engine exits when SDL cannot init.
	bool gpuRendererAvailable()
	{
		SDL_ResetHint(SDL_HINT_VIDEO_DRIVER);
		SDL_ResetHint(SDL_HINT_RENDER_DRIVER);
		bool available = false;
		if(SDL_InitSubSystem(SDL_INIT_VIDEO))
		{
			SDL_Window* window = SDL_CreateWindow("probe", 16, 16, SDL_WINDOW_HIDDEN);
			SDL_Renderer* renderer = window ? SDL_CreateRenderer(window, NULL) : NULL;
			available = renderer && SDL_strcmp(SDL_GetRendererName(renderer), SDL_SOFTWARE_RENDERER) != 0;
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
		return available;
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
