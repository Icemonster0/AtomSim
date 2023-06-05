#include "includes.h"
#include <cmath>

using namespace std;

vector<Atom> atom_list;
vector<Spring> spring_list;


void generate_atoms() {
    for(int i = 0; i < 10; i++) {
        atom_list.emplace_back(vec3(0, 0, -i), 1.0f);
        if(i) {
            spring_list.emplace_back(&atom_list[i], &atom_list[i-1], 1.0f);
        }
    }
}


void generate_trimesh(int width, int depth, int height) {
    Atom* grid[width][depth][height];
    float layer_height = sqrt(0.5); // height of a tetrahedron with an edge length of 1.0

    // add atoms
    for(int z = 0; z < height; z += 2) {
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < depth; y++) {
                Atom atom_bottom(vec3(x, y, z*layer_height), 1.0f);
                Atom atom_top(vec3(x+0.5, y+0.5, (z+1)*layer_height), 1.0f);
                atom_list.push_back(atom_bottom);
                atom_list.push_back(atom_top);
                grid[x][y][z] = &atom_bottom;
                grid[x][y][z+1] = &atom_top;
            }
        }
    }

    // add springs
    for(int z = 0; z < height; z++) {
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < depth; y++) {
                if(x < width-1)
                    spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z], 1.0f);
                if(y < height-1)
                    spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z], 1.0f);
                if(z) {
                    if(z % 2) {
                        spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], 1.0f);
                        spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z-1], 1.0f);
                        spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z-1], 1.0f);
                        spring_list.emplace_back(grid[x][y][z], grid[x+1][y+1][z-1], 1.0f);
                    } else {
                        spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], 1.0f);
                        spring_list.emplace_back(grid[x+1][y][z], grid[x][y][z-1], 1.0f);
                        spring_list.emplace_back(grid[x][y+1][z], grid[x][y][z-1], 1.0f);
                        spring_list.emplace_back(grid[x+1][y+1][z], grid[x][y][z-1], 1.0f);
                    }
                }
            }
        }
    }
}


int main() {

    // generate_atoms();
    generate_trimesh(10, 10, 10);

    for(auto atom : atom_list) {
        cout << "This atom's mass is " << atom.mass << " and it is at the coordinates " << endl
            << atom.pos.x << endl
            << atom.pos.y << endl
            << atom.pos.z << endl;
    }
    cout << "There are " << spring_list.size() << " springs and " << atom_list.size() << "atoms." << endl;
    for(auto spring : spring_list) {
        cout << "Calculated spring length: " << spring.calculate_length() << endl;
    }

    return 0;
}
