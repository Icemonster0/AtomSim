#include "includes.h"


void update_position(float delta_t) {
    #pragma omp parallel for schedule(static)
    for(auto &atom : mesh.atom_list) {
        if(!atom.fixed)
            atom.apply_velocity(delta_t);
    }
}


void update_velocity(float delta_t) {
    // #pragma omp parallel for schedule(static)
    for(auto &spring : mesh.spring_list) {
        spring.apply_force(delta_t);
    }
    #pragma omp parallel for schedule(static)
    for(auto &atom : mesh.atom_list) {
        atom.apply_gravity(delta_t);
        if(floor_enable) atom.collide_with_floor();
        atom.dampen(delta_t);
    }
}


float calculate_energy() {
    float energy = 0.0f;

    for(auto &atom : mesh.atom_list) {
        // Kinetic
        // float velocity = atom.velocity_length();
        energy += 0.5f * atom.mass * atom.velocity_sqr();

        // Potential (Gravity)
        energy += atom.mass * gravity * atom.pos.z;
    }
    for(auto &spring : mesh.spring_list) {
        // Potential (Springs)
        float l = spring.calculate_length();
        //  non-linear:
        if(l > spring.resting_length)
            energy += 0.5f * spring.k * (l - spring.resting_length) * (l - spring.resting_length);
        else
            energy += spring.resting_length * spring.k * ((l - spring.resting_length) + spring.resting_length * logf(spring.resting_length / l));
        //  linear:
        // energy += 0.5f * spring.k * (l - spring.resting_length) * (l - spring.resting_length);
    }

    return energy;
}


vec3 center_of_mass() {
    vec3 center;
    float total_mass = 0.0f;

    for(auto &atom : mesh.atom_list) {
        center = center + atom.pos * atom.mass;
        total_mass += atom.mass;
    }

    center = center / total_mass;

    return center;
}


void sim_step(float delta_t) {
    update_position(delta_t);
    update_velocity(delta_t);
}


void output(float time) {
    cout << time
        << " " << calculate_energy()
        << " " << center_of_mass().z
        // << " " << atom_list[555].vel.z
        // << " " << atom_list[867].vel.z
        << endl;
}


void start_sim() {
    float omega = sqrtf(spring_constant / atom_mass);
    float delta_t = step_size / omega;

    // first step
    update_velocity(delta_t / 2);

    for(int t = 0; t < sim_length / delta_t; t++) {
        sim_step(delta_t);
        output(t * delta_t);

        // render
        int steps_per_frame = time_per_frame / delta_t;
        if(t % steps_per_frame == 0 && render_enable) {
            render(t / steps_per_frame);
        }
    }

    if(save_cache) mesh.save_mesh_cache();
}
