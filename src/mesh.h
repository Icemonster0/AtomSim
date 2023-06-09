#pragma once

#include "atom.h"
#include "spring.h"
#include "util.h"


class Mesh {
    public:
        std::vector<Atom> atom_list;
        std::vector<Spring> spring_list;
        vec3 size;

        Mesh(vec3 p_size) : size(p_size) {
            if(load_cache) load_mesh_size();
            generate_trimesh((int)size.x, (int)size.y, (int)size.z);
            if(load_cache) load_mesh_cache();
        }

        void spring_cleanup() {
            for(int i = 0; i < spring_list.size(); i++) {
                if(spring_list[i].atom_a == nullptr || spring_list[i].atom_b == nullptr) {
                    spring_list.erase(spring_list.begin()+i);
                }
            }
        }

        void transform(vec3 center) {
            for(auto &atom : atom_list) {
                Atom rot_tmp = atom;

                // center around (0, 0, 0) -------------------------------------
                atom.pos.x -= center.x;
                atom.pos.y -= center.y;
                atom.pos.z -= center.z;

                // TWIST -------------------------------------------------------
                rot_tmp = atom;
                //  x-axis
                rot_tmp = atom;
                atom.pos.y = rot_tmp.pos.y * cos(mesh_twist.x * rot_tmp.pos.x) - rot_tmp.pos.z * sin(mesh_twist.x * rot_tmp.pos.x);
                atom.pos.z = rot_tmp.pos.y * sin(mesh_twist.x * rot_tmp.pos.x) + rot_tmp.pos.z * cos(mesh_twist.x * rot_tmp.pos.x);
                //  y-axis
                rot_tmp = atom;
                atom.pos.x = rot_tmp.pos.x * cos(mesh_twist.y * rot_tmp.pos.y) - rot_tmp.pos.z * sin(mesh_twist.y * rot_tmp.pos.y);
                atom.pos.z = rot_tmp.pos.x * sin(mesh_twist.y * rot_tmp.pos.y) + rot_tmp.pos.z * cos(mesh_twist.y * rot_tmp.pos.y);
                //  z-axis
                rot_tmp = atom;
                atom.pos.x = rot_tmp.pos.x * cos(mesh_twist.z * rot_tmp.pos.z) - rot_tmp.pos.y * sin(mesh_twist.z * rot_tmp.pos.z);
                atom.pos.y = rot_tmp.pos.x * sin(mesh_twist.z * rot_tmp.pos.z) + rot_tmp.pos.y * cos(mesh_twist.z * rot_tmp.pos.z);

                // SHEAR -------------------------------------------------------
                atom.pos.x += atom.pos.z * mesh_shear;

                // SCALE -------------------------------------------------------
                atom.pos.x *= mesh_stretch.x;
                atom.pos.y *= mesh_stretch.y;
                atom.pos.z *= mesh_stretch.z;

                // ROTATE ------------------------------------------------------
                //  x-axis
                rot_tmp = atom;
                atom.pos.y = rot_tmp.pos.y * cos(mesh_rotation.x) - rot_tmp.pos.z * sin(mesh_rotation.x);
                atom.pos.z = rot_tmp.pos.y * sin(mesh_rotation.x) + rot_tmp.pos.z * cos(mesh_rotation.x);
                //  y-axis
                rot_tmp = atom;
                atom.pos.x = rot_tmp.pos.x * cos(mesh_rotation.y) - rot_tmp.pos.z * sin(mesh_rotation.y);
                atom.pos.z = rot_tmp.pos.x * sin(mesh_rotation.y) + rot_tmp.pos.z * cos(mesh_rotation.y);
                //  z-axis
                rot_tmp = atom;
                atom.pos.x = rot_tmp.pos.x * cos(mesh_rotation.z) - rot_tmp.pos.y * sin(mesh_rotation.z);
                atom.pos.y = rot_tmp.pos.x * sin(mesh_rotation.z) + rot_tmp.pos.y * cos(mesh_rotation.z);

                // TRANSLATE ---------------------------------------------------
                atom.pos.x += mesh_offset.x;
                atom.pos.y += mesh_offset.y;
                atom.pos.z += mesh_offset.z;

                // offset to original position ---------------------------------
                atom.pos.x += center.x;
                atom.pos.y += center.y;
                atom.pos.z += center.z;
            }
        }

        bool atom_in_sphere(Atom* atom, vec3 center, float layer_height) {
            float xdiff = (center.x - atom->pos.x) / center.x / layer_height; // to unsquish the sphere
            float ydiff = (center.y - atom->pos.y) / center.y / layer_height;
            float zdiff = (center.z - atom->pos.z) / center.z;

            float l = sqrtf(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);

            return (l <= 1.0f);
        }

        bool spring_in_sphere(Atom* atom_a, Atom* atom_b) {
            bool result;
            if(sphere) result = (atom_a != nullptr && atom_b != nullptr);
            else result = true;

            return result;
        }

        void generate_trimesh(int width, int depth, int height) {
            Atom* grid[width][depth][height];
            float layer_height = sqrt(0.5); // height of a tetrahedron with an edge length of 1.0
            vec3 center = vec3(((float)width + 0.5) * 0.5f,
                               ((float)depth + 0.5) * 0.5f,
                               (float)height * layer_height * 0.5f);
            bool even = (height % 2 == 0);

            atom_list.reserve(height*width*depth);

            // add atoms
            for(int z = 0; z < height; z += 2) {
                for(int x = 0; x < width; x++) {
                    for(int y = 0; y < depth; y++) {
                        Atom bottom = Atom(vec3(x, y, z*layer_height), atom_mass, false);
                        Atom top = Atom(vec3(x+0.5, y+0.5, (z+1)*layer_height), atom_mass, false);

                        if(sphere && atom_in_sphere(&bottom, center, layer_height) || !sphere)
                            grid[x][y][z] = &atom_list.emplace_back(bottom);
                        else
                            grid[x][y][z] = nullptr;
                        if(z <= height-2) {
                            if(sphere && atom_in_sphere(&top, center, layer_height) || !sphere)
                                grid[x][y][z+1] = &atom_list.emplace_back(top);
                            else
                                grid[x][y][z+1] = nullptr;
                        }

                        if(z == 0 && fix_bottom) grid[x][y][z]->fix();
                        if(fix_top) {
                            if(z == height-2) grid[x][y][z+1]->fix();
                            if(z == height-1) grid[x][y][z]->fix();
                        }
                    }
                }
            }

            // add springs
            for(int z = 0; z < height; z++) {
                for(int x = 0; x < width; x++) {
                    for(int y = 0; y < depth; y++) {
                        bool canx = x < width-1;
                        bool cany = y < depth-1;

                        if(canx && spring_in_sphere(grid[x][y][z], grid[x+1][y][z]))
                            spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z], spring_constant);
                        if(cany && spring_in_sphere(grid[x][y][z], grid[x][y+1][z]))
                            spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z], spring_constant);
                        if(z) {
                            if(z % 2) {
                                if(spring_in_sphere(grid[x][y][z], grid[x][y][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], spring_constant);

                                if(canx && spring_in_sphere(grid[x][y][z], grid[x+1][y][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x+1][y][z-1], spring_constant);

                                if(cany && spring_in_sphere(grid[x][y][z], grid[x][y+1][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x][y+1][z-1], spring_constant);

                                if(canx && cany && spring_in_sphere(grid[x][y][z], grid[x+1][y+1][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x+1][y+1][z-1], spring_constant);

                            } else {
                                if(spring_in_sphere(grid[x][y][z], grid[x][y][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x][y][z-1], spring_constant);

                                if(x && spring_in_sphere(grid[x][y][z], grid[x-1][y][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x-1][y][z-1], spring_constant);

                                if(y && spring_in_sphere(grid[x][y][z], grid[x][y-1][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x][y-1][z-1], spring_constant);

                                if(x * y && spring_in_sphere(grid[x][y][z], grid[x-1][y-1][z-1]))
                                    spring_list.emplace_back(grid[x][y][z], grid[x-1][y-1][z-1], spring_constant);
                            }
                        }
                    }
                }
            }

            // transform
            transform(center);
        }

        void save_mesh_cache() {
            ofstream file;

            file.open(cache_path_save);

            file << mesh_size.x << " "
                 << mesh_size.y << " "
                 << mesh_size.z << " " << endl;

            for(auto atom : atom_list) {
                file << atom.pos.x << " " << atom.pos.y << " " << atom.pos.z << " "
                     << atom.vel.x << " " << atom.vel.y << " " << atom.vel.z << " " << endl;
            }

            file.close();
        }

        void load_mesh_size() {
            ifstream file;

            file.open(cache_path_load);
            file >> size.x >> size.y >> size.z;
            file.close();
        }

        void load_mesh_cache() {
            ifstream file;

            file.open(cache_path_load);

            float dump;
            file >> dump >> dump >> dump;

            for(auto &atom : atom_list) {
                file >> atom.pos.x >> atom.pos.y >> atom.pos.z
                     >> atom.vel.x >> atom.vel.y >> atom.vel.z;
            }

            file.close();
        }
};
