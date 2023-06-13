#pragma once

// Global settings for the simulation


// SIMULATION ------------------------------------------------------------------
    // General
    const float sim_length = 30;
    const float step_size = 0.01;
    const float spring_constant = 5000.0f;
    const float atom_mass = 1.0f;
    const float gravity = 1.0f;
    const float damping = 0;
    const bool breaking = false;
    const float breaking_distance = 1.05;
    const bool floor_enable = true;

    // Save and load cache
    const bool load_cache = false;
    const std::string cache_path_load = "cache/cache1";
    const bool save_cache = false;
    const std::string cache_path_save = "cache/cache1";

    // Temperature
    // Intended use: disable gravity, damping, fix_top, fix_bottom,
    // enable rendering;
    const bool temperature_sim = false;
    const bool render_distribution = false; // false: render avg temps per layer
    const float starting_temp = 1.0f;
    const int temp_height = 17;
    const float temp_step = 0.01;
    const float max_temperature = 1;
    const int plot_height = 70; // active if render_distribution = true
    const std::string temp_output_file = "graphs/output1";
    // All layers up to temp_height start with rand(0, starting_temp), the rest
    // starts with 0; plots out the temperature distribution or average
    // temperature per layer to output_path


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
    const vec3 mesh_rotation = vec3(0.0, 0.5, 0.2);
    const vec3 mesh_offset = vec3(0, 0, 4);


// RENDER ----------------------------------------------------------------------
    // General
    const bool render_enable = true;
    const std::string output_path = "output/";
    const bool output_png = true; // false: output pnm
    const bool clean_output_directory = true; // recommended
    const int res_X = 800;
    const int res_Y = 600;
    const float scale = 20;
    const float offset_X = -5;
    const float offset_Y = -5;
    const float time_per_frame = 0.1f;

    // Surface
    const bool show_surface = false;
    const bool flat_sides = true; // also rounds mesh height down to an odd number
    const vec3 light_direction = vec3(-1, -1, -0.5);

    // Springs
    const bool show_tension = false;
    const float color_scale = 4.0f;

    // Atoms
    const bool show_atoms = true;
    const color atom_draw_color = color(1.0f, 1.0f, 1.0f);
    const float atom_radius = 2.5;

    // Misc colors
    const color background_color = color(0.25f, 0.25f, 0.25f);
    const color floor_color = color(0.15f, 0.15f, 0.15f);
