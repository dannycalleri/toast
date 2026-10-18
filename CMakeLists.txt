cmake_minimum_required(VERSION 3.24)
project(Toast LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TOAST_FETCH_DEPS "Download and build SDL3, SDL3_image and SDL3_ttf instead of using installed packages" OFF)
set(TOAST_SANITIZERS "" CACHE STRING "Comma-separated sanitizers to build with, e.g. address,undefined")
option(TOAST_BUILD_TESTS "Build the Toast test suite" ON)

if(TOAST_FETCH_DEPS)
    include(FetchContent)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_TIF OFF CACHE BOOL "" FORCE)
    set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
    set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
    set(SDLTTF_PLUTOSVG OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.18 GIT_SHALLOW TRUE)
    FetchContent_Declare(SDL3_image GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-3.4.8 GIT_SHALLOW TRUE GIT_SUBMODULES_RECURSE TRUE)
    FetchContent_Declare(SDL3_ttf GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-3.2.2 GIT_SHALLOW TRUE GIT_SUBMODULES_RECURSE TRUE)
    FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_ttf)
else()
    find_package(SDL3 3.2 CONFIG)
    find_package(SDL3_image CONFIG)
    find_package(SDL3_ttf CONFIG)
    if(NOT SDL3_FOUND OR NOT SDL3_image_FOUND OR NOT SDL3_ttf_FOUND)
        message(FATAL_ERROR
            "SDL3, SDL3_image and SDL3_ttf were not all found. Install them "
            "(macOS: brew install sdl3 sdl3_image sdl3_ttf) or configure with -DTOAST_FETCH_DEPS=ON.")
    endif()
endif()

function(toast_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W3)
    else()
        target_compile_options(${target} PRIVATE -Wall)
    endif()
endfunction()

if(TOAST_SANITIZERS AND NOT MSVC)
    add_compile_options(-fsanitize=${TOAST_SANITIZERS} -fno-sanitize-recover=all -fno-omit-frame-pointer)
    add_link_options(-fsanitize=${TOAST_SANITIZERS})
endif()

# --- framework ---------------------------------------------------------------

file(GLOB TOAST_SOURCES CONFIGURE_DEPENDS ToastFramework/*.cpp)
add_library(toast STATIC ${TOAST_SOURCES})
target_include_directories(toast PUBLIC ToastFramework ToastFramework/rapidxml-1.13)
target_link_libraries(toast PUBLIC SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)
toast_warnings(toast)

# Assets are loaded relative to the executable (TF::resolvePath),
# so each game gets its assets copied next to it.
function(toast_copy_assets target)
    foreach(asset IN LISTS ARGN)
        get_filename_component(name ${asset} NAME)
        if(IS_DIRECTORY ${asset})
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${asset} $<TARGET_FILE_DIR:${target}>/${name})
        else()
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${asset} $<TARGET_FILE_DIR:${target}>/${name})
        endif()
    endforeach()
    # a shared SDL3 (e.g. from vcpkg) must sit next to the .exe to load;
    # with static SDL the list is empty and copy_if_different would fail
    if(WIN32)
        set(dlls $<TARGET_RUNTIME_DLLS:${target}>)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "$<$<BOOL:${dlls}>:${CMAKE_COMMAND};-E;copy_if_different;${dlls};$<TARGET_FILE_DIR:${target}>>"
            COMMAND_EXPAND_LISTS)
    endif()
endfunction()

# --- games -------------------------------------------------------------------

set(DEMO_ASSETS
    ${CMAKE_SOURCE_DIR}/DEMO/04B_03__.TTF
    ${CMAKE_SOURCE_DIR}/DEMO/animazione_loop_goblin.png
    ${CMAKE_SOURCE_DIR}/DEMO/controls.png
    ${CMAKE_SOURCE_DIR}/DEMO/tile_caverna.png)

add_executable(demo DEMO/main.cpp DEMO/Game.cpp DEMO/GameWorld.cpp)
target_link_libraries(demo PRIVATE toast)
toast_warnings(demo)
toast_copy_assets(demo ${DEMO_ASSETS})

set(ROGUELIKE_ASSETS
    ${CMAKE_SOURCE_DIR}/ROGUELIKE/04B_03__.TTF
    ${CMAKE_SOURCE_DIR}/ROGUELIKE/assets
    ${CMAKE_SOURCE_DIR}/ROGUELIKE/rooms)

# everything but main(), so the tests can build levels
add_library(roguelike_game STATIC
    ROGUELIKE/Bullet.cpp ROGUELIKE/Cat.cpp ROGUELIKE/Effects.cpp ROGUELIKE/Game.cpp ROGUELIKE/GameWorld.cpp ROGUELIKE/Hud.cpp ROGUELIKE/Key.cpp ROGUELIKE/Level.cpp
    ROGUELIKE/Maze.cpp ROGUELIKE/Movement.cpp ROGUELIKE/Player.cpp ROGUELIKE/Room.cpp ROGUELIKE/RoomTemplates.cpp
    ROGUELIKE/ScreenShake.cpp)
target_include_directories(roguelike_game PUBLIC ROGUELIKE)
target_link_libraries(roguelike_game PUBLIC toast)
toast_warnings(roguelike_game)

add_executable(roguelike ROGUELIKE/main.cpp)
target_link_libraries(roguelike PRIVATE roguelike_game)
toast_warnings(roguelike)
toast_copy_assets(roguelike ${ROGUELIKE_ASSETS})

# ASCII art sources -> PNG sheets. The PNGs are committed, so only an art change
# needs this: cmake --build build --target roguelike_art
add_library(pixelart_lib STATIC tools/PixelArt.cpp)
target_include_directories(pixelart_lib PUBLIC tools)
target_link_libraries(pixelart_lib PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)
toast_warnings(pixelart_lib)

add_executable(pixelart EXCLUDE_FROM_ALL tools/pixelart_main.cpp)
target_link_libraries(pixelart PRIVATE pixelart_lib)
toast_warnings(pixelart)

file(GLOB ROGUELIKE_ART_SHEETS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/ROGUELIKE/art/*.txt)
list(REMOVE_ITEM ROGUELIKE_ART_SHEETS ${CMAKE_SOURCE_DIR}/ROGUELIKE/art/palette.txt)
add_custom_target(roguelike_art
    COMMAND pixelart ${CMAKE_SOURCE_DIR}/ROGUELIKE/assets
        ${CMAKE_SOURCE_DIR}/ROGUELIKE/art/palette.txt ${ROGUELIKE_ART_SHEETS})

# --- tests -------------------------------------------------------------------

if(TOAST_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
