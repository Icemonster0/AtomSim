#include "includes.h"

using namespace std;


Mesh mesh = Mesh(vec3(mesh_size.x, mesh_size.y, mesh_size.z));


void generate_atoms() {
    for(int i = 0; i < 10; i++) {
        mesh.atom_list.emplace_back(vec3(0, 0, -i), atom_mass, false);
        if(i) {
            mesh.spring_list.emplace_back(&mesh.atom_list[i], &mesh.atom_list[i-1], spring_constant);
        }
    }
    mesh.atom_list[0].fix();
}


void clean_dir() {
    if(system(("rm " + output_path + "*").c_str())) {
        cout << "Failed to clean output directory." << endl;
    }
}


void initial_condition() {
    // atom_list[10].vel.x -= 50;
    // atom_list[10].vel.y += 20;
    // atom_list[10].vel.z += 30;

    // generate_atoms();
    // atom_list[9].vel.x += 3;
}


int main() {

    srand(time(NULL));

    if(clean_output_directory)
        clean_dir();

    initial_condition();

    start_sim();

    // for(auto atom : atom_list) {
    //     cout << "This atom's mass is " << atom.mass << ", it is at the coordinates " << endl
    //         << atom.pos.x << endl
    //         << atom.pos.y << endl
    //         << atom.pos.z << endl;
    //     cout << atom.fixed << endl;
    // }
    // cout << "There are " << spring_list.size() << " springs and " << atom_list.size() << " atoms." << endl;
    // for(auto spring : spring_list) {
    //     cout << ": " << spring.calculate_length() << endl;
    //     // cout << "spring: " << spring.atom_a->pos.z << endl;
    // }

    return 0;
}
