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
    atom_list.reserve(height*width*depth);
    for(int z = 0; z < height; z += 2) {
        for(int x = 0; x < width; x++) {
            for(int y = 0; y < depth; y++) {
                grid[x][y][z] = &atom_list.emplace_back(vec3(x, y, z*layer_height), 1.0f);
                grid[x][y][z+1] = &atom_list.emplace_back(vec3(x+0.5, y+0.5, (z+1)*layer_height), 1.0f);
                // cout << grid[x][y][z]->pos.x << endl;
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
                    spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z], 1.0f);
                if(cany)
                    spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z], 1.0f);
                if(z) {
                    if(z % 2) {
                        spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], 1.0f);
                        if(canx)
                            spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z-1], 1.0f);
                        if(cany)
                            spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z-1], 1.0f);
                        if(canx && cany)
                            spring_list.emplace_back(grid[x][y][z], grid[x+1][y+1][z-1], 1.0f);
                    } else {
                        spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], 1.0f);
                        if(x)
                            spring_list.emplace_back(grid[x][y][z], grid[x-1][y][z-1], 1.0f);
                        if(y)
                            spring_list.emplace_back(grid[x][y][z], grid[x][y-1][z-1], 1.0f);
                        if(x * y)
                            spring_list.emplace_back(grid[x][y][z], grid[x-1][y-1][z-1], 1.0f);
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
    cout << "There are " << spring_list.size() << " springs and " << atom_list.size() << " atoms." << endl;
    for(auto spring : spring_list) {
        cout << ": " << spring.calculate_length() << endl;
        // cout << "spring: " << spring.atom_a->pos.z << endl;
    }

    return 0;
}
