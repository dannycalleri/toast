add_executable(toast_tests test_main.cpp test_input.cpp test_tf.cpp test_grid.cpp test_tilemap.cpp test_engine.cpp)
target_link_libraries(toast_tests PRIVATE toast)
toast_warnings(toast_tests)
toast_copy_assets(toast_tests ${DEMO_ASSETS})

foreach(name
        grid_out_of_range_tiles_are_ignored
        grid_bitstring_rows_set_only_their_own_bits
        grid_csv_rows_keep_their_last_column
        grid_copy_is_independent_of_the_original
        tilemap_visible_range_covers_only_the_view
        tilemap_draws_set_tiles_where_the_camera_shows_them
        tilemap_linear_filtering_does_not_bleed_the_next_atlas_tile
        input_keymap
        input_out_of_range_key_is_never_down
        tf_resolve_path_is_relative_to_executable
        tf_texture_cache_miss_does_not_insert
        engine_loads_assets_and_renders
        engine_translucent_canvas_content_blends_once
        engine_canvas_alpha_scales_color_and_coverage
        engine_text_is_drawn_inside_its_canvas
        engine_mouse_is_mapped_through_letterbox
        engine_key_pressed_once_down_until_released
        engine_frame_time_is_capped
        engine_window_opens_at_the_largest_whole_scale_that_fits)
    add_test(NAME ${name} COMMAND toast_tests ${name})
    set_tests_properties(${name} PROPERTIES SKIP_RETURN_CODE 77)
endforeach()

add_executable(roguelike_tests test_main.cpp test_art.cpp test_maze.cpp test_rooms.cpp test_roguelike.cpp)
target_link_libraries(roguelike_tests PRIVATE roguelike_game pixelart_lib)
target_compile_definitions(roguelike_tests PRIVATE TOAST_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
toast_warnings(roguelike_tests)
toast_copy_assets(roguelike_tests ${ROGUELIKE_ASSETS})

foreach(name
        roguelike_room_doors_follow_the_mask
        roguelike_rocks_are_solid_and_bones_are_not
        roguelike_maze_rooms_use_several_templates
        roguelike_locked_room_is_behind_a_locked_door
        roguelike_level_without_templates_builds_nothing
        maze_has_the_requested_rooms_all_reachable
        maze_doors_are_symmetric_and_lead_to_rooms
        maze_same_seed_gives_the_same_maze
        maze_locked_room_is_the_farthest_dead_end
        maze_extra_doors_avoid_the_start_and_the_locked_room
        maze_generation_takes_at_most_two_steps_per_room
        maze_of_ten_thousand_rooms_is_fast
        maze_invalid_sizes_give_an_empty_maze
        art_committed_sheets_match_their_sources
        art_unknown_colour_key_is_reported_with_its_line
        art_flash_frames_are_white_copies
        art_palette_rejects_bad_colours_and_repeated_keys
        rooms_every_template_parses
        rooms_bad_template_is_skipped_and_logged
        rooms_every_floor_cell_is_reachable_from_every_door)
    add_test(NAME ${name} COMMAND roguelike_tests ${name})
    set_tests_properties(${name} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 60)
endforeach()
