add_executable(toast_tests test_main.cpp test_input.cpp test_tf.cpp test_grid.cpp test_engine.cpp)
target_link_libraries(toast_tests PRIVATE toast)
toast_warnings(toast_tests)
toast_copy_assets(toast_tests ${DEMO_ASSETS})

foreach(name
        grid_out_of_range_tiles_are_ignored
        grid_bitstring_rows_set_only_their_own_bits
        grid_csv_rows_keep_their_last_column
        input_keymap
        input_out_of_range_key_is_never_down
        tf_resolve_path_is_relative_to_executable
        tf_texture_cache_miss_does_not_insert
        engine_loads_assets_and_renders
        engine_translucent_canvas_content_blends_once
        engine_canvas_alpha_scales_color_and_coverage
        engine_text_is_drawn_inside_its_canvas
        engine_mouse_is_mapped_through_letterbox
        engine_key_pressed_once_down_until_released)
    add_test(NAME ${name} COMMAND toast_tests ${name})
    set_tests_properties(${name} PROPERTIES SKIP_RETURN_CODE 77)
endforeach()

add_executable(roguelike_tests test_main.cpp test_roguelike.cpp)
target_link_libraries(roguelike_tests PRIVATE roguelike_game)
toast_warnings(roguelike_tests)
toast_copy_assets(roguelike_tests ${ROGUELIKE_ASSETS})

foreach(name
        roguelike_every_room_file_loads_with_its_doors
        roguelike_missing_room_file_gives_an_empty_room
        roguelike_dungeons_generate_without_overlapping_rooms)
    add_test(NAME ${name} COMMAND roguelike_tests ${name})
    set_tests_properties(${name} PROPERTIES SKIP_RETURN_CODE 77)
endforeach()
