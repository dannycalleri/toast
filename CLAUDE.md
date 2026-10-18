# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Toast Framework: a 2D C++17 game framework modeled on FlashPunk (AS3), built on **SDL3** (+ SDL3_image, SDL3_ttf). Everything lives in `namespace Toast`. The repo has:

- `ToastFramework/` — the framework, built as the static library `toast`.
- `DEMO/` — minimal sample game; the intended template for new games.
- `ROGUELIKE/` — "Cat Crypt", the larger sample. `Maze` builds the level with the Growing Tree algorithm (O(N), no retries; the locked room is the farthest one-door room), `Level` turns it into `Room`s, and each `Room` fills its 20x12 tiles from an ASCII template in `ROGUELIKE/rooms/rooms.txt` (its header documents the format). Its pixel art is written as text in `ROGUELIKE/art/` and turned into the committed `assets/*.png` by `cmake --build build --target roguelike_art` (the `pixelart` tool in `tools/`); a test fails when a PNG no longer matches its source. It builds as the `roguelike_game` library (everything but `main.cpp`, so tests can link it) plus the `roguelike` executable.
- `tests/` — a tiny self-contained test runner (`ToastTest.h`) with two executables: `toast_tests` (framework) and `roguelike_tests` (maze generation, room templates, rooms, pixel art).

## Building and testing

```bash
brew install cmake sdl3 sdl3_image sdl3_ttf      # macOS; elsewhere use a package manager/vcpkg, or -DTOAST_FETCH_DEPS=ON
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure        # all tests
ctest --test-dir build -R input_keymap            # one test (or: build/tests/toast_tests input_keymap)
./build/demo                                       # or ./build/roguelike (G generates a maze)
```

- Sanitizers: `-DTOAST_SANITIZERS=undefined` (or `address,undefined`). On this macOS 26 / Apple clang 17 machine ASan hangs at process start even for an empty `main`, so use UBSan only here.
- New framework `.cpp` files are picked up automatically (`file(GLOB ... CONFIGURE_DEPENDS)`); new test files and test names must be added to `tests/CMakeLists.txt` (each test is registered with CTest by name).
- Engine tests run headless: call `useHeadlessVideo()` (`tests/HeadlessVideo.h`) before creating the `Engine` to get SDL's `dummy` video driver and `software` renderer; pixels can be read back with `SDL_RenderReadPixels`. The software renderer ignores alpha when blending into render targets and snaps where a GPU filters, so tests of translucent canvases and texture filtering call `gpuRendererAvailable()` (same header) and `SKIP` without a GPU (exit 77, reported by ctest as "Not Run").
- Game assets are listed per target in `CMakeLists.txt` and copied next to the executable by `toast_copy_assets`; a new asset must be added there.

## Architecture

**Global state via `TF` (`TF.h`).** A class of static members acts as the service locator: `TF::engine` (its `window`/`renderer` are the SDL handles), `TF::world` (the single active World), `TF::camera`, `TF::console`, `TF::font`, `TF::elapsed` (seconds since last frame), `TF::width/height` (logical resolution), `TF::debug`, `TF::paused`, and a texture cache (`TF::getTexture/addTexture`, freed by `destroyAllTextures`). `TF::resolvePath` makes asset paths relative to the executable (`SDL_GetBasePath`); every loader goes through it. `Input` (`ToastInput.h`) is likewise all-static; SDL scancodes are mapped to the `TOAST_KEY_*` codes (`ToastKeycodes.h`, Allegro-compatible values) by `Input::keyFromScancode`.

**Bootstrapping a game** (see `DEMO/`): `main.cpp` includes `<SDL3/SDL_main.h>` (only there — it must be in exactly one translation unit), creates a `Game` subclass of `Engine` and calls `run()`. The `Game` constructor passes the logical resolution to `Engine(width, height, fullscreen)` and assigns `TF::world = new MyWorld()`. Game code includes `Toast.h` or individual `Toast*.h` headers; `ToastCommon.h` holds the SDL includes and forward declarations.

**Engine (`ToastEngine.cpp`)**: `initEngine()` creates the window + renderer (vsync on) and sets `SDL_SetRenderLogicalPresentation(LETTERBOX)`, so the game always draws at its logical size and SDL scales/letterboxes to any window size or pixel density. `EventManager::checkEvents` polls SDL events and runs mouse events through `SDL_ConvertEventToRenderCoordinates`, so `Input::mouseX/Y` are already in game coordinates. `TF::elapsed` is capped at `Engine::MAX_FRAME_SECONDS`. Textures filter linearly unless the game calls `setPixelArt(true)` before loading its art (ROGUELIKE does). If init fails the engine shows a message box and exits. `~Engine` deletes world, console, font and textures *before* destroying the renderer — anything owning an `SDL_Texture`/`TTF_Font` must die first.

**World → Entity → Graphic hierarchy (FlashPunk model)**:
- `World` owns `entities` and standalone `graphicsList`; override `update()` (call the base to tick children) and, to draw around the world (e.g. a camera shake), `render()`. Render order is insertion order: all entities, then world graphics. `World::remove()` **deletes** the entity at once, so during an update use `World::removeLater()`, which deletes it after every entity has updated; the World destructor deletes everything it holds.
- `Entity` has position, hitbox (`width/height/originX/originY`), `active/visible/collidable`, and one or more `Graphic`s.
- `Graphic` is the base for drawables: `Image`, `Spritemap` (+ `Anim`), `Tilemap`, `Canvas`, `Text`, and `Emitter` (FlashPunk-style particles: named types on frames of one texture, straight-line motion plus gravity, alpha and scale tweened over each particle's life; dead particles are swapped out, so the pool never shrinks). Key fields: `scrollX/scrollY` (parallax factor vs `TF::camera`; 1 = normal), `relative` (draw relative to parent Entity), `flipped`, `alpha`, and `angle` (radians, pivot at the top-left corner). All drawing goes through `Graphics::*` (`ToastGraphics.cpp`).
- `Canvas` is an SDL render-target texture. Drawing into it switches the render target and restores the previous one; its contents are premultiplied (it uses `SDL_BLENDMODE_BLEND_PREMULTIPLIED`, and `DrawCanvas` applies `alpha` to color and alpha). `Text` draws into a canvas. `Tilemap` is not a canvas: it stores a tile index per cell and draws only the tiles inside the view each frame (`Tilemap::visibleRange`), so later `setTile` calls replace a tile rather than blending over it.
- Collision: `Mask` base with `Hitbox` and `Grid` (tile-based, `loadFromString` CSV); collision dispatch between mask types is only partly implemented. `SpriteSheet`/`Sprite` are deprecated stubs.

**Debugging aids**: `TF::debug = true` enables the in-game `Console` overlay (ENTER toggles pause + entity selection/dragging). `TOAST_ERROR(msg)` logs via `SDL_LogError`; `fatalError` also shows a message box. `TOAST_SAFE_DELETE(x)` is the delete-and-null idiom.

## Conventions

- Files are `Toast<Class>.h/.cpp`, classes are unprefixed inside `namespace Toast`; include guards are `TOAST<NAME>_H`; every framework file starts with the "Toast 2D game framework" copyright banner.
- Tabs for indentation, Allman braces, raw pointers with manual ownership (no smart pointers) — the public API stays this way on purpose.
- `rapidxml-1.13/` is vendored third-party code; don't modify it.
- Some sample-game identifiers/assets are in Italian (e.g. `tile_caverna.png`, `animazione_loop_goblin.png`).
