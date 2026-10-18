add_executable(toast_tests test_main.cpp test_emitter.cpp test_input.cpp test_tf.cpp test_grid.cpp test_tilemap.cpp test_engine.cpp test_world.cpp)
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
        engine_window_opens_at_the_largest_whole_scale_that_fits
        engine_image_defaults_draw_at_full_size_and_opacity
        engine_image_scale_multiplies_its_drawn_size
        engine_image_alpha_blends_over_what_is_below
        emitter_adds_one_particle_per_emit
        emitter_particles_expire_after_their_duration
        emitter_particles_move_in_a_straight_line
        emitter_reuses_the_slots_of_dead_particles
        emitter_unknown_type_adds_nothing_and_logs_once
        emitter_draws_particles_scaled_and_centred_on_their_point
        world_entity_can_remove_itself_during_update
        world_queued_entity_is_deleted_once)
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
        roguelike_kid_stops_at_a_wall_and_slides_along_it
        roguelike_walking_through_a_door_slides_to_the_next_room
        roguelike_walking_along_a_wall_past_a_door_stays_in_the_room
        roguelike_holding_fire_spits_bullets_from_the_mouth_at_the_cooldown_rate
        roguelike_bullet_breaks_on_a_wall_into_particles
        roguelike_bullet_breaks_at_the_end_of_its_range
        roguelike_new_game_removes_the_bullets_of_the_old_maze
        roguelike_shots_are_never_closer_than_the_cooldown
        roguelike_shake_trauma_is_capped_and_decays_to_zero
        roguelike_spawning_cat_is_harmless_and_bullets_pass_over_it
        roguelike_pouncer_crouches_before_it_leaps
        roguelike_zoomie_closes_in_on_the_kid
        roguelike_entering_a_new_room_spawns_cats_that_are_not_active_yet
        roguelike_cat_finds_its_way_around_a_wall
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
        art_variant_redraws_the_sheet_with_swapped_keys
        rooms_every_template_parses
        rooms_bad_template_is_skipped_and_logged
        rooms_every_floor_cell_is_reachable_from_every_door)
    add_test(NAME ${name} COMMAND roguelike_tests ${name})
    set_tests_properties(${name} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 60)
endforeach()
