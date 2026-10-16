#include "ToastTest.h"
#include "ToastInput.h"

using namespace Toast;

TOAST_TEST(input_keymap)
{
	CHECK(Input::keyFromScancode(SDL_SCANCODE_A) == TOAST_KEY_A);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_Z) == TOAST_KEY_Z);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_0) == TOAST_KEY_0);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_1) == TOAST_KEY_1);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_9) == TOAST_KEY_9);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_KP_0) == TOAST_KEY_PAD_0);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_KP_9) == TOAST_KEY_PAD_9);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_F1) == TOAST_KEY_F1);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_F12) == TOAST_KEY_F12);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_RETURN) == TOAST_KEY_ENTER);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_ESCAPE) == TOAST_KEY_ESCAPE);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_UP) == TOAST_KEY_UP);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_LSHIFT) == TOAST_KEY_LSHIFT);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_RSHIFT) == TOAST_KEY_RSHIFT);
	CHECK(Input::keyFromScancode(SDL_SCANCODE_F13) == 0);
}

TOAST_TEST(input_out_of_range_key_is_never_down)
{
	CHECK(!Input::pressed(TOAST_KEY_MAX));
	CHECK(!Input::down(TOAST_KEY_MAX + 100));
}
