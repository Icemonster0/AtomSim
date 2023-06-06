#include "includes.h"


void update_position(float delta_t) {
    for(auto atom : atom_list) {
        atom.apply_velocity(delta_t);
    }
}

void update_velocity(float delta_t) {
    for(auto spring : spring_list) {
        spring.apply_force(delta_t);
    }
}

void sim_step(float delta_t) {
    update_position(delta_t);
    update_velocity(delta_t);
}

void start_sim() {
    float delta_t = sqrtf(spring_constant / atom_mass);

    // first step
    update_velocity(delta_t / 2);

    for(int t = 0; t < steps; t++) {
        sim_step(delta_t);
    }
}
