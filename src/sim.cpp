#include "includes.h"


void update_position(float delta_t) {
    #pragma omp parallel for schedule(static)
    for(auto &atom : mesh.atom_list) {
        if(!atom.fixed)
            atom.apply_velocity(delta_t);
    }
}


void break_springs() {
    for(auto &atom : mesh.atom_list) {
        int working_springs = 0;
        for(auto &spring_i : atom.springs) {
            if(mesh.spring_list[spring_i].k) working_springs++;
        }
        if(working_springs <= 2) {
            for(auto &spring_i : atom.springs)
                mesh.spring_list[spring_i].k = 0;
        }
    }
}


void update_velocity(float delta_t) {
    #pragma omp parallel for schedule(static)
    for(auto &spring : mesh.spring_list) {
        spring.calculate_acceleration(delta_t);
    }
    if(breaking)
        break_springs();
    for(auto &spring : mesh.spring_list) {
        spring.apply_acceleration();
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


void initialize_temperatures() {
    for(int i = 0; i < temp_height * mesh_size.x * mesh_size.y; i++) {
        vec3 dir = vec3((float)rand()/RAND_MAX*2-1, (float)rand()/RAND_MAX*2-1, (float)rand()/RAND_MAX*2-1);
        float l = sqrtf(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);

        dir = dir / l;

        float size = (float)rand()/RAND_MAX * starting_temp;

        mesh.atom_list[i].vel = dir * size;
    }
}


int count_atoms_with_temp(float min_temp, float max_temp) {
    int count = 0;

    for(auto &atom : mesh.atom_list) {
        float l = sqrtf(atom.vel.x*atom.vel.x + atom.vel.y*atom.vel.y + atom.vel.z*atom.vel.z);
        if(l >= min_temp && l < max_temp) {
            count++;
        }
    }

    return count;
}


void output_temp_curve(float time) {
    ofstream file;
    file.open(temp_output_file);
    if(!file.is_open()) {
        cout << "Failed to open file in  sim.cpp: output_temp_curve" << endl;
        exit(1);
    }

    for(float temp = 0.0f; temp < max_temperature; temp += temp_step) {
        int count = count_atoms_with_temp(temp, temp + temp_step);

        file << temp << " " << count << endl;
    }

    file.close();

    if(system(("gnuplot -e \"set terminal png size " + to_string(res_X) + "," + to_string(res_Y) + "; set xr [0:" + to_string(starting_temp) + "]; set yr [0:" + to_string(plot_height) + "]; set output '" + output_path + "/" + to_string(time) + ".png'; plot '" + temp_output_file + "' u 1:2\"").c_str())) {
        cout << "Failed to plot graph. Read the README for a possible solution." << endl;
        exit(1);
    }
}


float avg_layer_temp(int layer) {
    float sum = 0;

    for(int i = layer * mesh_size.x * mesh_size.y; i < (layer+1) * mesh_size.x * mesh_size.y; i++) {
        float vel = mesh.atom_list[i].vel.x*mesh.atom_list[i].vel.x
        + mesh.atom_list[i].vel.y*mesh.atom_list[i].vel.y
        + mesh.atom_list[i].vel.z*mesh.atom_list[i].vel.z;
        sum += vel;
    }

    return sum / (mesh_size.x * mesh_size.y);
}


void plot_avg_temps(float time) {
    ofstream file;
    file.open(temp_output_file);
    if(!file.is_open()) {
        cout << "Failed to open file in  sim.cpp: plot_avg_temps" << endl;
        exit(1);
    }

    for(int i = 0; i < mesh_size.z; i++) {
        file << i << " " << avg_layer_temp(i) << endl;
    }

    file.close();

    if(system(("gnuplot -e \"set terminal png size " + to_string(res_X) + "," + to_string(res_Y) + "; set xr [0:" + to_string(mesh_size.z) + "]; set yr [0:" + to_string(starting_temp) + "]; set output '" + output_path + "/" + to_string(time) + ".png'; plot '" + temp_output_file + "' u 1:2\"").c_str())) {
        cout << "Failed to plot graph. Read the README for a possible solution." << endl;
        exit(1);
    }
}


void sim_step(float delta_t) {
    update_position(delta_t);
    update_velocity(delta_t);
}


void output(float time) {
    cout << time
        << " " << calculate_energy()
        << " " << center_of_mass().z;
    cout << endl;
}


void start_sim() {
    float omega = sqrtf(spring_constant / atom_mass);
    float delta_t = step_size / omega;

    if(temperature_sim)
        initialize_temperatures();

    // first step
    update_velocity(delta_t / 2);

    for(int t = 0; t < sim_length / delta_t; t++) {
        sim_step(delta_t);
        output(t * delta_t);

        // render
        int steps_per_frame = time_per_frame / delta_t;
        if(t % steps_per_frame == 0 && render_enable) {
            if(temperature_sim) {
                if(render_distribution)
                    output_temp_curve(t * delta_t);
                else
                    plot_avg_temps(t * delta_t);
            } else
                render(t / steps_per_frame);

        }
    }

    if(save_cache) mesh.save_mesh_cache();
}
