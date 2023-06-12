#pragma once

// Global settings for the simulation


// SIMULATION ------------------------------------------------------------------
    // General
    const float sim_length = 30;
    const float step_size = 0.01;
    const float spring_constant = 5000.0f;
    const float atom_mass = 1.0f;
    const float gravity = 0.0f;
    const float damping = 0;
    const bool floor_enable = false;

    // Save and load cache
    const bool load_cache = false;
    const std::string cache_path_load = "cache/cache1"; // active if  load_cache = true
    const bool save_cache = false;
    const std::string cache_path_save = "cache/cache1"; // active if  save_cache = true

    // Temperature
    // Intended use: disable gravity, damping, fix_top, fix_bottom and sphere,
    // enable rendering;
    const bool temperature_sim = true;
    const float starting_temp = 1.0f;
    const int temp_height = 9;
    const float temp_step = 0.01;
    const float max_temperature = 1;
    const std::string temp_output_file = "graphs/output1";
    // All layers up to temp_height start with rand(0, starting_temp), the rest
    // starts with 0; prints out the temperature distribution to output_path


// INITIAL CONDITION -----------------------------------------------------------
    // Mesh size
    const vec3 mesh_size = vec3(10, 10, 10);

    // Modifiers
    const bool fix_top = false;
    const bool fix_bottom = false;
    const bool sphere = false;

    // Transformations
    const vec3 mesh_twist = vec3(0, 0, 0);
    const float mesh_shear = 0.0f;
    const vec3 mesh_stretch = vec3(1, 1, 1);
    const vec3 mesh_rotation = vec3(0.0, 0.0, 0.0);
    const vec3 mesh_offset = vec3(0, 0, 4);


// RENDER ----------------------------------------------------------------------
    // General
    const bool render_enable = true;
    const std::string output_path = "output/";
    const bool output_png = true; // false: output pnm
    const bool clean_output_directory = true;
    const int res_X = 800;
    const int res_Y = 600;
    const float scale = 30;
    const float offset_X = -10;
    const float offset_Y = -5;
    const float time_per_frame = 0.001f;

    // Surface
    const bool show_surface = true;
    const bool flat_sides = true; // active if  show_surface = true; rounds mesh height down to an odd number
    const vec3 light_direction = vec3(-1, -1, -0.5); // active if  show_surface = true

    // Springs
    const bool show_tension = false;
    const float color_scale = 4.0f; // active if  show_tension = true

    // Atoms
    const bool show_atoms = false;
    const color draw_color = color(0.8f, 0.8f, 0.8f); // active if  show_atoms = true

    // Misc colors
    const color background_color = color(0.35f, 0.35f, 0.35f);
    const color floor_color = color(0.15f, 0.15f, 0.15f); // active if  floor_enable = true
