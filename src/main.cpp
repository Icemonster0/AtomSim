#include "includes.h"

using namespace std;

vector<Atom> atom_list;
vector<Spring> spring_list;


void generate_atoms() {
    for(int i = 0; i < 10; i++) {
        atom_list.emplace_back(vec3(0, 0, -i), atom_mass, false);
        if(i) {
            spring_list.emplace_back(&atom_list[i], &atom_list[i-1], spring_constant);
        }
    }
}


void generate_trimesh(int width, int depth, int height) {
    Atom* grid[width][depth][height];
    float layer_height = sqrt(0.5); // height of a tetrahedron with an edge length of 1.0

    // add atoms
    atom_list.reserve(height*width*depth);
    for(int z = 0; z < height; z += 2) {
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < depth; y++) {
                grid[x][y][z] = &atom_list.emplace_back(vec3(x, y, z*layer_height), atom_mass, false);
                grid[x][y][z+1] = &atom_list.emplace_back(vec3(x+0.5, y+0.5, (z+1)*layer_height), atom_mass, false);
                // cout << grid[x][y][z]->pos.x << endl;
                if(!z) grid[x][y][z]->fix();
            }
        }
    }

    // add springs
    for(int z = 0; z < height; z++) {
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < depth; y++) {
                // cout << x << " " << grid[x][y][z]->pos.x << "  ";
                // cout << y << " " << grid[x][y][z]->pos.y << "  ";
                // cout << z << " " << grid[x][y][z]->pos.z << endl;
                bool canx = x < width-1;
                bool cany = y < depth-1;

                if(canx)
                    spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z], spring_constant);
                if(cany)
                    spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z], spring_constant);
                if(z) {
                    if(z % 2) {
                        spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], spring_constant);
                        if(canx)
                            spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z-1], spring_constant);
                        if(cany)
                            spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z-1], spring_constant);
                        if(canx && cany)
                            spring_list.emplace_back(grid[x][y][z], grid[x+1][y+1][z-1], spring_constant);
                    } else {
                        spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], spring_constant);
                        if(x)
                            spring_list.emplace_back(grid[x][y][z], grid[x-1][y][z-1], spring_constant);
                        if(y)
                            spring_list.emplace_back(grid[x][y][z], grid[x][y-1][z-1], spring_constant);
                        if(x * y)
                            spring_list.emplace_back(grid[x][y][z], grid[x-1][y-1][z-1], spring_constant);
                    }
                }
            }
        }
    }
}


void update_position(float delta_t) {
    for(auto atom : atom_list) {
        atom.apply_velocity(delta_t);
    }
}


void update_velocity(float delta_t) {
    for(auto spring : spring_list) {
        spring.apply_force(delta_t);
    }
    for(auto atom : atom_list) {
        atom.apply_gravity(delta_t);
    }
}


void sim_step(float delta_t) {
    update_position(delta_t);
    update_velocity(delta_t);
    cout << "vel z: " << atom_list.at(200).vel.z << endl;
}


void start_sim() {
    float delta_t = sqrtf(spring_constant / atom_mass);
    delta_t = 1.0f;

    // first step
    update_velocity(delta_t / 2);

    for(int t = 0; t < steps; t++) {
        sim_step(delta_t);
    }
}


int main() {

    // generate_atoms();
    generate_trimesh(10, 10, 10);

    start_sim();

    for(auto atom : atom_list) {
        cout << "This atom's mass is " << atom.mass << ", it is at the coordinates " << endl
            << atom.pos.x << endl
            << atom.pos.y << endl
            << atom.pos.z << endl;
        cout << atom.fixed << endl;
    }
    cout << "There are " << spring_list.size() << " springs and " << atom_list.size() << " atoms." << endl;
    // for(auto spring : spring_list) {
    //     cout << ": " << spring.calculate_length() << endl;
    //     // cout << "spring: " << spring.atom_a->pos.z << endl;
    // }

    return 0;
}
