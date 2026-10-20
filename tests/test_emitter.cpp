#include "ToastTest.h"
#include "HeadlessVideo.h"
#include "ReadPixel.h"
#include "Toast.h"
#include "ToastEmitter.h"
#include "ToastGraphics.h"
#include "ToastTexture.h"

#include <string>
#include <vector>

using namespace Toast;

namespace
{
	std::vector<std::string> logged;

	void captureLog(void*, int, SDL_LogPriority, const char* message)
	{
		logged.push_back(message);
	}

	Engine* startEngine()
	{
		useHeadlessVideo();
		Engine* engine = new Engine(64, 64);
		SDL_Surface* surface = SDL_CreateSurface(8, 4, SDL_PIXELFORMAT_RGBA8888);
		SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGBA(surface, 255, 255, 255, 255));
		Texture* texture = new Texture();
		texture->tex = SDL_CreateTextureFromSurface(engine->renderer, surface);
		texture->setWidth(8);
		texture->setHeight(4);
		SDL_DestroySurface(surface);
		TF::addTexture("dots", texture);
		return engine;
	}

	void run(Emitter& emitter, float seconds, int frames)
	{
		TF::elapsed = seconds / frames;
		for(int i = 0; i < frames; i++)
			emitter.update();
	}
}

TOAST_TEST(emitter_adds_one_particle_per_emit)
{
	Engine* engine = startEngine();
	Emitter emitter("dots", 4, 4);
	emitter.newType("spark", std::vector<int>(1, 0));
	for(int i = 0; i < 10; i++)
		emitter.emit("spark", 0, 0);
	CHECK(emitter.particleCount() == 10);
	delete engine;
}

TOAST_TEST(emitter_particles_expire_after_their_duration)
{
	Engine* engine = startEngine();
	Emitter emitter("dots", 4, 4);
	emitter.newType("spark", std::vector<int>(1, 0));
	emitter.setMotion("spark", 0, 10, 0.5f);
	emitter.emit("spark", 0, 0);

	run(emitter, 0.4f, 4);
	CHECK(emitter.particleCount() == 1);
	run(emitter, 0.2f, 2);
	CHECK(emitter.particleCount() == 0);
	delete engine;
}

TOAST_TEST(emitter_particles_move_in_a_straight_line)
{
	Engine* engine = startEngine();
	Emitter emitter("dots", 4, 4);
	emitter.newType("spark", std::vector<int>(1, 0));
	emitter.setMotion("spark", 0, 40, 1.0f);
	emitter.emit("spark", 5, 7);

	run(emitter, 0.5f, 5);
	float x, y;
	emitter.particlePosition(0, x, y);
	CHECK(SDL_fabsf(x - 25) <= 1.0f);
	CHECK(SDL_fabsf(y - 7) <= 1.0f);

	Emitter falling("dots", 4, 4);
	falling.newType("drop", std::vector<int>(1, 0));
	falling.setMotion("drop", 0, 0, 2.0f);
	falling.setGravity("drop", 100);
	falling.emit("drop", 0, 0);
	run(falling, 1.0f, 10);
	falling.particlePosition(0, x, y);
	CHECK(SDL_fabsf(y - 50) <= 1.0f);
	delete engine;
}

TOAST_TEST(emitter_reuses_the_slots_of_dead_particles)
{
	Engine* engine = startEngine();
	Emitter emitter("dots", 4, 4);
	emitter.newType("spark", std::vector<int>(1, 0));
	emitter.setMotion("spark", 0, 10, 0.1f);

	for(int i = 0; i < 100; i++)
		emitter.emit("spark", 0, 0);
	size_t capacity = emitter.particleCapacity();
	run(emitter, 0.2f, 2);
	CHECK(emitter.particleCount() == 0);

	for(int i = 0; i < 100; i++)
		emitter.emit("spark", 0, 0);
	CHECK(emitter.particleCount() == 100);
	CHECK(emitter.particleCapacity() == capacity);
	delete engine;
}

TOAST_TEST(emitter_unknown_type_adds_nothing_and_logs_once)
{
	Engine* engine = startEngine();
	Emitter emitter("dots", 4, 4);

	SDL_LogOutputFunction previous;
	void* previousData;
	SDL_GetLogOutputFunction(&previous, &previousData);
	logged.clear();
	SDL_SetLogOutputFunction(captureLog, NULL);
	emitter.emit("nope", 0, 0);
	emitter.emit("nope", 0, 0);
	emitter.setAlpha("nope", 1, 0);
	SDL_SetLogOutputFunction(previous, previousData);

	CHECK(emitter.particleCount() == 0);
	CHECK(logged.size() == 1);
	CHECK(logged[0].find("nope") != std::string::npos);
	delete engine;
}

TOAST_TEST(emitter_draws_particles_scaled_and_centred_on_their_point)
{
	Engine* engine = startEngine();
	Emitter emitter("dots", 4, 4);
	emitter.newType("spark", std::vector<int>(1, 1));
	emitter.setMotion("spark", 0, 0, 1.0f);
	emitter.setScale("spark", 2, 2);
	emitter.emit("spark", 20, 20);

	engine->resetRenderingBuffer();
	Graphics::Clear(0, 0, 0);
	emitter.render();

	// 4x4 at scale 2 is 8x8, from 16 to 24 on both axes
	CHECK(readGamePixel(engine->renderer, 16, 16).r == 255);
	CHECK(readGamePixel(engine->renderer, 23, 23).r == 255);
	CHECK(readGamePixel(engine->renderer, 15, 20).r == 0);
	CHECK(readGamePixel(engine->renderer, 24, 20).r == 0);

	emitter.alpha = 0.0f;
	Graphics::Clear(0, 0, 0);
	emitter.render();
	CHECK(readGamePixel(engine->renderer, 20, 20).r == 0);
	delete engine;
}
