#pragma once

#include "util.h"

class Atom {
    public:
        vec3 pos;
        vec3 vel;
        float mass;
        bool fixed;

        Atom(vec3 p_pos, float p_mass, bool p_fixed) : pos(p_pos), vel(vec3(0, 0, 0)), mass(p_mass), fixed(p_fixed) {
        }

        void fix() {
          fixed = true;
        }

        void apply_velocity(float delta_t) {
            pos.x += vel.x * delta_t;
            pos.y += vel.y * delta_t;
            pos.z += vel.z * delta_t;
        }

        void apply_gravity(float delta_t) {
            vel.z -= gravity * delta_t;
        }
};
