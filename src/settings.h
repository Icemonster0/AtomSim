#pragma once

// Global settings for the simulation


// Simulation
const int steps = 100000;
const float step_size = 0.01;
const float spring_constant = 1500.0f;
const float atom_mass = 1.0f;
const float gravity = 0.7f;
const bool floor_enable = true;


// Initial condition
const vec3 mesh_size = vec3(15, 10, 10);

const bool fix_top = false;
const bool fix_bottom = false;
const bool sphere = false;

const vec3 mesh_twist = vec3(0.1, 0, 0);
const float mesh_shear = 0.0f;
const vec3 mesh_stretch = vec3(1, 1, 1);
const vec3 mesh_rotation = vec3(0, 0, 0.7);
const vec3 mesh_offset = vec3(0, 0, 4);


// Render
const bool render_enable = true;
const std::string output_path = "output/";
const int res_x = 100;
const int res_y = 100;
const float scale = 5;
const float offset_x = -15;
const float offset_y = -25;
const int steps_per_frame = 300;

const color background_color = color(0.3f, 0.3f, 0.3f);
const bool show_tension = true;
const float color_scale = 4.0f; // active if  show_tension = true
const color draw_color = color(0.8f, 0.8f, 0.8f); // active if  show_tension = false
const color floor_color = color(0.2f, 0.2f, 0.2f); // active if  floor_enable = true
