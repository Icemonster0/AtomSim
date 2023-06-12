#pragma once

#include "atom.h"
#include <cmath>

using namespace std;


class Spring {
    public:
        Atom* atom_a;
        Atom* atom_b;
        vec3 acceleration;
        float resting_length;
        float k;

        Spring(Atom* p_atom_a, Atom* p_atom_b, float pk) :
            atom_a(p_atom_a), atom_b(p_atom_b), k(pk), acceleration(vec3(0, 0, 0)) {
                resting_length = calculate_length();
        }

        float calculate_length() {
            // return std::hypotf(std::hypotf(atom_a->pos.x - atom_b->pos.x, atom_a->pos.y - atom_b->pos.y), atom_a->pos.z - atom_b->pos.z);
            float xdiff = atom_a->pos.x - atom_b->pos.x;
            float ydiff = atom_a->pos.y - atom_b->pos.y;
            float zdiff = atom_a->pos.z - atom_b->pos.z;
            // std::cout << xdiff << " " << ydiff << " " << zdiff << " " << std::endl;
            // std::cout << std::endl << "atom_a->pos.x " << atom_a->pos.x << std::endl;
            float l = sqrtf(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
            return l;
        }

        float force_curve(float l) {
            if(l > 0) {
                return k * l;
            }
            else {
                return k * resting_length * (-1.0f / (l / resting_length + 1.0f) + 1.0f);
            }
        }

        vec3 calculate_force() {
            float xdiff = atom_a->pos.x - atom_b->pos.x;
            float ydiff = atom_a->pos.y - atom_b->pos.y;
            float zdiff = atom_a->pos.z - atom_b->pos.z;

            float l = sqrtf(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
            //  non-linear:
            float force_str = force_curve(l - resting_length);
            // cout << "force length: " << force_str << endl;
            //  linear:
            // float force_str = k * (l - resting_length);
            float force_over_l = force_str / l;

            return vec3(xdiff * force_over_l, ydiff * force_over_l, zdiff * force_over_l);
        }

        void calculate_acceleration(float delta_t) {
            vec3 force = calculate_force();

            acceleration = vec3(force.x / atom_a->mass * delta_t, force.y / atom_a->mass * delta_t, force.z / atom_a->mass * delta_t);

            // atom_a->vel.x -= force.x / atom_a->mass * delta_t;
            // atom_a->vel.y -= force.y / atom_a->mass * delta_t;
            // atom_a->vel.z -= force.z / atom_a->mass * delta_t;
            //
            // atom_b->vel.x += force.x / atom_b->mass * delta_t;
            // atom_b->vel.y += force.y / atom_b->mass * delta_t;
            // atom_b->vel.z += force.z / atom_b->mass * delta_t;
        }

        void apply_acceleration() {
            atom_a->vel = atom_a->vel - acceleration;
            atom_b->vel = atom_b->vel + acceleration;
        }
};
