add_executable(toast_tests test_main.cpp test_input.cpp test_tf.cpp test_engine.cpp)
target_link_libraries(toast_tests PRIVATE toast)
toast_warnings(toast_tests)
toast_copy_assets(toast_tests ${DEMO_ASSETS})

foreach(name
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
