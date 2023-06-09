#pragma once

// Global settings for the simulation


// Simulation
const float sim_length = 2;
const float step_size = 0.001;
const float spring_constant = 3500.0f;
const float atom_mass = 1.0f;
const float gravity = 50.0f;
const float damping = 0;
const bool floor_enable = false;

const bool load_cache = true;
const std::string cache_path_load = "cache/cache2"; // active if  load_cache = true
const bool save_cache = false;
const std::string cache_path_save = "cache/cache2"; // active if  save_cache = true


// Initial condition
const vec3 mesh_size = vec3(5, 5, 25);

const bool fix_top = false;
const bool fix_bottom = false;
const bool sphere = false;

const vec3 mesh_twist = vec3(0, 0, 0);
const float mesh_shear = 0.0f;
const vec3 mesh_stretch = vec3(1, 1, 1);
const vec3 mesh_rotation = vec3(0, 0, 0);
const vec3 mesh_offset = vec3(0, 0, 20);


// Render
const bool render_enable = true;
const std::string output_path = "output/";
const int res_x = 60;
const int res_y = 150;
const float scale = 3;
const float offset_x = -25;
const float offset_y = -25;
const float time_per_frame = 0.01f;

const color background_color = color(0.3f, 0.3f, 0.3f);
const bool show_tension = true;
const float color_scale = 4.0f; // active if  show_tension = true
const color draw_color = color(0.8f, 0.8f, 0.8f); // active if  show_tension = false
const color floor_color = color(0.2f, 0.2f, 0.2f); // active if  floor_enable = true
