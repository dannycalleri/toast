Toast Framework
===============

Toast is an easy-to-use 2D game framework for C++, inspired by
[FlashPunk](https://github.com/useflashpunk/FlashPunk). If you know AS3 and
FlashPunk, you already know how a Toast game is put together: a `World` full of
`Entity` objects, each drawn by a `Graphic`. Toast runs natively on macOS, Linux
and Windows on top of [SDL3](https://libsdl.org).

It's perfect if you want to switch from AS3 to C++ in the most painless way
possible, and for learning C++ while doing what you love: making games.

<p>
  <img src="docs/demo.png" alt="The DEMO game: two animated goblins in a cave" width="49%">
  <img src="docs/roguelike.png" alt="The ROGUELIKE game: four generated rooms joined by their doors" width="49%">
</p>

Features
--------------------------------------
- **FlashPunk-style structure**: `World`, `Entity` and `Graphic`, with global access
  to the engine, the current world, the camera and the frame time through `TF`.
- **Graphics**: images, animated sprite sheets (`Spritemap`), tilemaps that draw
  only the tiles on screen, TrueType text, and off-screen canvases. Alpha,
  rotation and flipping are built in.
- **Camera** with per-graphic scroll factors for parallax.
- **Any window size**: the game draws at a fixed resolution that you choose, and
  Toast scales it to the window or screen with black bars, sharp on HiDPI and
  Retina displays. Mouse coordinates are always in game coordinates. Pixel-art
  games call `setPixelArt(true)` in their `Engine` constructor, before loading any
  art, to get nearest-neighbour scaling.
- **Input**: keyboard (`TOAST_KEY_*` codes; "pressed this frame" and "held down")
  and mouse.
- **Collision**: test an entity's hitbox against a rectangle with
  `Entity::collideRect`, and store solid tiles in a `Grid` you query with `getTile`.
  Collision between `Mask` types (`Hitbox` against `Hitbox` or `Grid`) is not
  finished yet and always reports no collision.
- **Debug console**: press ENTER to pause the game, see every entity's hitbox, and
  select or drag entities with the mouse. It is off by default; turn it on in your
  `Engine` constructor with `TF::debug = true; TF::console->enable();`.
- **Assets** load from the executable's directory, so games run from any working
  directory.

Quick start
--------------------------------------
You need CMake and SDL3 with SDL3_image and SDL3_ttf.

macOS (Homebrew):

    brew install cmake sdl3 sdl3_image sdl3_ttf
    cmake -B build
    cmake --build build
    ./build/demo
    ./build/roguelike

Linux and Windows: install SDL3, SDL3_image and SDL3_ttf with your package manager
or vcpkg, then run the same CMake commands. If you do not have them, configure with
`cmake -B build -DTOAST_FETCH_DEPS=ON`, and CMake downloads and builds them for you.

The build copies each game's assets next to its executable.

### The sample games

| | DEMO | ROGUELIKE |
|---|---|---|
| What it shows | Sprites, animation, a scrolling tilemap, text, the debug console | Procedural dungeon generation from rooms made in Ogmo Editor |
| Controls | Arrow keys move the goblin, WASD moves the camera | G generates a dungeon, WASD moves the camera |
| Both | ENTER toggles the debug console (drag to select, Shift+drag to move), ESC quits | |

Make your own game
--------------------------------------
A game is an `Engine` subclass that sets the first `World`:

```cpp
#include "Toast.h"
#include "ToastEntity.h"
#include "ToastImage.h"
#include "ToastInput.h"
#include "ToastWorld.h"
#include <SDL3/SDL_main.h>   // in main.cpp only

using namespace Toast;

class MyWorld : public World
{
public:
	MyWorld()
	{
		player = new Entity(100, 100);
		player->graphic = new Image("player.png");
		add(player);
	}

	virtual void update()
	{
		World::update();

		if(Input::down(TOAST_KEY_LEFT))  player->x -= 200 * TF::elapsed;
		if(Input::down(TOAST_KEY_RIGHT)) player->x += 200 * TF::elapsed;
		if(Input::pressed(TOAST_KEY_ESCAPE)) TF::engine->onExit();
	}

private:
	Entity* player;
};

class MyGame : public Engine
{
public:
	MyGame() : Engine(1280, 720)   // game resolution; scaled to fit the window
	{
		TF::world = new MyWorld();
	}
};

int main(int argc, char* argv[])
{
	MyGame game;
	return game.run();
}
```

Add it to `CMakeLists.txt` and list its assets. The engine's default font,
`04B_03__.TTF`, must ship with every game:

```cmake
add_executable(mygame MyGame/main.cpp)
target_link_libraries(mygame PRIVATE toast)
toast_copy_assets(mygame
    ${CMAKE_SOURCE_DIR}/MyGame/player.png
    ${CMAKE_SOURCE_DIR}/DEMO/04B_03__.TTF)
```

The world owns what you add to it: `World::remove()` and the world's destructor
delete entities and graphics. The `DEMO` directory is a good place to start from.

Project layout
--------------------------------------
| Directory | Contents |
|---|---|
| `ToastFramework/` | The framework, built as the `toast` static library |
| `DEMO/` | The minimal sample game |
| `ROGUELIKE/` | The dungeon generator sample (`rooms/` holds the Ogmo Editor rooms) |
| `tests/` | Unit and integration tests |

Tests
--------------------------------------
    ctest --test-dir build --output-on-failure      # all tests
    ctest --test-dir build -R input_keymap          # one test

Most tests run without a display. A few check GPU rendering and are reported as
skipped on machines without a GPU. Configure with `-DTOAST_SANITIZERS=undefined`
(or `address,undefined`) to build with sanitizers.

Platforms
--------------------------------------
Toast is built and tested on macOS (Apple Silicon). The CMake build is written for
Linux and Windows too, but nobody has tried it there yet. Reports and fixes are
welcome!

Toast started in 2013 on Allegro 5 and Visual Studio 2012. In 2026 it moved to
SDL3 and CMake, keeping its game-facing API.

Contribution
--------------------------------------
The code is thoroughly commented, but if you run into any problem, let me know! :)

In the spirit of open source software development, I always encourage community code contribution, so let's make a fork!
Just please let me know if you do something interesting with it.

License
--------------------------------------
[MIT License](http://opensource.org/licenses/mit-license.php)

&copy; 2013 Danny Calleri &lt;daniele.calleri@gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

For more information, see the accompanying "MIT-LICENSE.txt" file.
