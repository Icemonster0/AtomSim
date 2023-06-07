#pragma once

// Global settings for the simulation

// Simulation
const int steps = 1000;
const float step_size = 0.1;
const float spring_constant = 100.0f;
const float atom_mass = 1.0f;
const float gravity = 0.0f;

// Render
const std::string output_path = "output/";
const int res_x = 100;
const int res_y = 100;
const float scale = 7;
const float offset_x = -15;
const float offset_y = -25;
const int steps_per_frame = 1;
const color background_color = color(0.3f, 0.3f, 0.3f);
const color draw_color = color(0.8f, 0.8f, 0.8f);
