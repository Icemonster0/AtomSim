#pragma once

#include "atom.h"
#include "spring.h"
#include "util.h"


class Mesh {
    public:
        std::vector<Atom> atom_list;
        std::vector<Spring> spring_list;
        std::vector<tri> tri_list;
        vec3 size;

        Mesh(vec3 p_size) : size(p_size) {
            if(load_cache) load_mesh_size();
            if(show_surface && flat_sides && (int)size.z % 2 == 0) size.z -= 1;
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

        Atom* get_closest_atom(vec3 point) {
            Atom* closest;
            float dist = 1000000.0f;

            for(Atom &atom : atom_list) {
                float xdiff = point.x - atom.pos.x;
                float ydiff = point.y - atom.pos.y;
                float zdiff = point.z - atom.pos.z;

                float l = xdiff*xdiff + ydiff*ydiff + zdiff*zdiff;

                if(l < dist) {
                    dist = l;
                    closest = &atom;
                }
            }

            if(dist < 1.0f)
                return closest;
            else
                return nullptr;
        }

        void generate_trimesh(int width, int depth, int height) {
            // I apologize for this monster function, it's just that c++ hates
            // 3D arrays and I didn't want to have to deal with passing
            // them to smaller functions.

            Atom* grid[width][depth][height];
            float layer_height = sqrt(0.5); // height of a tetrahedron with an edge length of 1.0
            vec3 center = vec3(((float)width + 0.5) * 0.5f,
                               ((float)depth + 0.5) * 0.5f,
                               (float)height * layer_height * 0.5f);
            // bool even_height = (height % 2 == 0);

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

            // add tris
            if(show_surface && !sphere) {
                // top and bottom
                for(int x = 0; x < width-1; x++) {
                    for(int y = 0; y < depth-1; y++) {
                        // bottom
                        tri_list.emplace_back(grid[x][y][0], grid[x+1][y+1][0], grid[x][y+1][0]);
                        tri_list.emplace_back(grid[x][y][0], grid[x+1][y][0], grid[x+1][y+1][0]);

                        // top
                        tri_list.emplace_back(grid[x][y][height-1], grid[x][y+1][height-1], grid[x+1][y+1][height-1]);
                        tri_list.emplace_back(grid[x][y][height-1], grid[x+1][y+1][height-1], grid[x+1][y][height-1]);
                    }
                }

                // sides
                int z_step = flat_sides ? 2 : 1;

                for(int z = 0; z < height-1; z += z_step) {
                    for(int x = 0; x < width-1; x++) {
                        // south
                        tri_list.emplace_back(grid[x][0][z], grid[x][0][z+z_step], grid[x+1][0][z+z_step]);
                        tri_list.emplace_back(grid[x][0][z], grid[x+1][0][z+z_step], grid[x+1][0][z]);

                        // north
                        tri_list.emplace_back(grid[x][depth-1][z], grid[x+1][depth-1][z+z_step], grid[x][depth-1][z+z_step]);
                        tri_list.emplace_back(grid[x][depth-1][z], grid[x+1][depth-1][z], grid[x+1][depth-1][z+z_step]);
                    }

                    for(int y = 0; y < depth-1; y++) {
                        // west
                        tri_list.emplace_back(grid[0][y][z], grid[0][y+1][z+z_step], grid[0][y][z+z_step]);
                        tri_list.emplace_back(grid[0][y][z], grid[0][y+1][z], grid[0][y+1][z+z_step]);

                        // east
                        tri_list.emplace_back(grid[width-1][y][z], grid[width-1][y][z+z_step], grid[width-1][y+1][z+z_step]);
                        tri_list.emplace_back(grid[width-1][y][z], grid[width-1][y+1][z+z_step], grid[width-1][y+1][z]);
                    }

                    if(flat_sides && z == height-1) {
                        z_step = 1;
                    }
                }
            }
            if(show_surface && sphere) {
                // Create temporary wrapping atoms

                std::vector<Atom> tmp_atoms;
                tmp_atoms.reserve(height*width*depth);
                int tmp_grid[width][depth][height];
                std::vector<tri> tmp_tris;

                // top and bottom
                for(int x = 0; x < width; x++) {
                    for(int y = 0; y < depth; y++) {
                        // bottom
                        tmp_grid[x][y][0] = tmp_atoms.size();
                        tmp_atoms.emplace_back(vec3(x, y, 0), atom_mass, false);
                        // top
                        tmp_grid[x][y][height-1] = tmp_atoms.size();
                        tmp_atoms.emplace_back(vec3(x, y, height-1), atom_mass, false);
                    }
                }

                //sides
                for(int z = 0; z < height; z++) {
                    for(int x = 0; x < width; x++) {
                        // south
                        tmp_grid[x][0][z] = tmp_atoms.size();
                        tmp_atoms.emplace_back(vec3(x, 0, z*layer_height), atom_mass, false);
                        // north
                        tmp_grid[x][depth-1][z] = tmp_atoms.size();
                        tmp_atoms.emplace_back(vec3(x, depth-1, z*layer_height), atom_mass, false);
                    }

                    for(int y = 0; y < depth; y++) {
                        // west
                        tmp_grid[0][y][z] = tmp_atoms.size();
                        tmp_atoms.emplace_back(vec3(0, y, z*layer_height), atom_mass, false);
                        // east
                        tmp_grid[width-1][y][z] = tmp_atoms.size();
                        tmp_atoms.emplace_back(vec3(width-1, y, z*layer_height), atom_mass, false);
                    }
                }

                std::vector<Atom*> tmp_atom_ptr(tmp_atoms.size());
                std::vector<bool> tmp_atom_final(tmp_atoms.size());

                for(int i = 0; i < tmp_atom_ptr.size(); i++) {
                    tmp_atom_ptr[i] = &tmp_atoms[i];
                    tmp_atom_final[i] = false;
                }

                // Fill in wrapping with tris

                // top and bottom
                for(int x = 0; x < width-1; x++) {
                    for(int y = 0; y < depth-1; y++) {
                        // bottom
                        tmp_tris.emplace_back(tmp_grid[x][y][0], tmp_grid[x+1][y+1][0], tmp_grid[x][y+1][0]);
                        tmp_tris.emplace_back(tmp_grid[x][y][0], tmp_grid[x+1][y][0], tmp_grid[x+1][y+1][0]);

                        // top
                        tmp_tris.emplace_back(tmp_grid[x][y][height-1], tmp_grid[x][y+1][height-1], tmp_grid[x+1][y+1][height-1]);
                        tmp_tris.emplace_back(tmp_grid[x][y][height-1], tmp_grid[x+1][y+1][height-1], tmp_grid[x+1][y][height-1]);
                    }
                }

                // sides

                for(int z = 0; z < height-1; z++) {
                    for(int x = 0; x < width-1; x++) {
                        // south
                        tmp_tris.emplace_back(tmp_grid[x][0][z], tmp_grid[x][0][z+1], tmp_grid[x+1][0][z+1]);
                        tmp_tris.emplace_back(tmp_grid[x][0][z], tmp_grid[x+1][0][z+1], tmp_grid[x+1][0][z]);

                        // north
                        tmp_tris.emplace_back(tmp_grid[x][depth-1][z], tmp_grid[x+1][depth-1][z+1], tmp_grid[x][depth-1][z+1]);
                        tmp_tris.emplace_back(tmp_grid[x][depth-1][z], tmp_grid[x+1][depth-1][z], tmp_grid[x+1][depth-1][z+1]);
                    }

                    for(int y = 0; y < depth-1; y++) {
                        // west
                        tmp_tris.emplace_back(tmp_grid[0][y][z], tmp_grid[0][y+1][z+1], tmp_grid[0][y][z+1]);
                        tmp_tris.emplace_back(tmp_grid[0][y][z], tmp_grid[0][y+1][z], tmp_grid[0][y+1][z+1]);

                        // east
                        tmp_tris.emplace_back(tmp_grid[width-1][y][z], tmp_grid[width-1][y][z+1], tmp_grid[width-1][y+1][z+1]);
                        tmp_tris.emplace_back(tmp_grid[width-1][y][z], tmp_grid[width-1][y+1][z+1], tmp_grid[width-1][y+1][z]);
                    }
                }

                // Shrink wrapping
                for(auto &atom : atom_list) {
                    atom.pos = atom.pos - center;
                }
                for(auto &atom : tmp_atoms) {
                    atom.pos = atom.pos - center;
                }

                // float decrement = clamp(1 / max(max(mesh_size.x, mesh_size.y), mesh_size.z), 1.0f, 100.0f);
                float decrement = 0.99f;

                for(int i = 0; i < tmp_atom_ptr.size(); i++) {
                    for(float factor = 1; factor >= 0.005; factor *= decrement) {
                        Atom* atom_ptr = get_closest_atom(tmp_atom_ptr[i]->pos * factor);
                        if(atom_ptr != nullptr) {
                            tmp_atom_ptr[i] = atom_ptr;
                            tmp_atom_final[i] = true;
                            break;
                        }
                    }
                }

                for(auto &atom : atom_list) {
                    atom.pos = atom.pos + center;
                }

                // Clean up tris
                for(int i = 0; i < tmp_tris.size(); i++) {
                    tmp_tris[i].fill_pointers(tmp_atom_ptr.data());

                    if(tmp_tris[i].a != tmp_tris[i].b && tmp_tris[i].b != tmp_tris[i].c && tmp_tris[i].c != tmp_tris[i].a
                       && tmp_atom_final[tmp_tris[i].ai] && tmp_atom_final[tmp_tris[i].bi] && tmp_atom_final[tmp_tris[i].ci]) {
                        tri_list.push_back(tmp_tris[i]);
                    }

                    // for(int i = 0; i < tri_list.size(); i++) {
                    //     cout << "p: " << tri_list[i].a << " " << tri_list[i].b << " " << tri_list[i].c << endl;
                    //     cout << "i: " << tri_list[i].ai << " " << tri_list[i].bi << " " << tri_list[i].ci << endl;
                    // }
                }

            }

            // store spring_i-s in atoms
            for(auto &atom : atom_list) {
                for(int i = 0; i < spring_list.size(); i++) {
                    if(spring_list[i].atom_a == &atom || spring_list[i].atom_b == &atom) {
                        atom.springs.push_back(i);
                    }
                }
            }

            // transform
            transform(center);
        }

        void save_mesh_cache() {
            ofstream file;

            file.open(cache_path_save);
            if(!file.is_open()) {
                cout << "Failed to open file in  mesh.h: Mesh::save_mesh_cache" << endl;
                exit(1);
            }

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
            if(!file.is_open()) {
                cout << "Failed to open file in  mesh.h: Mesh::load_mesh_size" << endl;
                exit(1);
            }

            file >> size.x >> size.y >> size.z;
            file.close();
        }

        void load_mesh_cache() {
            ifstream file;

            file.open(cache_path_load);
            if(!file.is_open()) {
                cout << "Failed to open file in  mesh.h: Mesh::load_mesh_cache" << endl;
                exit(1);
            }

            float dump;
            file >> dump >> dump >> dump;

            for(auto &atom : atom_list) {
                file >> atom.pos.x >> atom.pos.y >> atom.pos.z
                     >> atom.vel.x >> atom.vel.y >> atom.vel.z;
            }

            file.close();
        }
};
