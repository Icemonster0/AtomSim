#pragma once

// Global settings for the simulation


// SIMULATION ------------------------------------------------------------------
    // General
    const float sim_length = 100;
    const float step_size = 0.01;
    const float spring_constant = 8000.0f;
    const float atom_mass = 1.0f;
    const float gravity = 1.0f;
    const float damping = 0.0;
    const bool breaking = false;
    const float breaking_distance = 1.05;
    const bool floor_enable = true;

    // Save and load cache
    const bool load_cache = false;
    const std::string cache_path_load = "cache/cache1";
    const bool save_cache = true;
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
    const vec3 mesh_size = vec3(30, 30, 30);

    // Shape
    const bool tetrahedron = false; // false: use mesh_size to generate block
    const float tetrahedron_size = 1.0f; // only size, no subdivisions

    // Modifiers
    const bool fix_top = false;
    const bool fix_bottom = false;
    const bool sphere = true;

    // Transformations
    const vec3 mesh_twist = vec3(0, 0, 0);
    const float mesh_shear = 0.0f;
    const vec3 mesh_stretch = vec3(1, 1, 1);
    const vec3 mesh_rotation = vec3(0.1, 0.3, 0.1);
    const vec3 mesh_offset = vec3(0, 0, 10);


// RENDER ----------------------------------------------------------------------
    // General
    const bool render_enable = true;
    const std::string output_path = "output/";
    const bool output_png = true; // false: output pnm
    const bool clean_output_directory = true; // recommended
    const int res_X = 1600;
    const int res_Y = 1200;
    const float scale = 60;
    const float offset_X = -15;
    const float offset_Y = -10;
    const float time_per_frame = 0.17f;

    // Surface
    const bool show_surface = true;
    const bool flat_sides = true; // also rounds mesh height down to an odd number
    const vec3 light_direction = vec3(-1, -1, -0.5);
    const bool fancy_shader = true;
    const bool colored = false;
    const color surface_color = color(0.72, 0.57, 0.41);
    const float randomize_color = 0.05f;

    // Springs
    const bool show_tension = false;
    const float color_scale = 4.0f;

    // Atoms
    const bool show_atoms = false;
    const color atom_draw_color = color(0.5, 1.0, 0.4);
    const float atom_radius = 2.5;

    // Misc colors
    const color background_color = color(0.25f, 0.25f, 0.25f);
    const color floor_color = color(0.15f, 0.15f, 0.15f);
