#pragma once

#include "util.h"


class Atom {
    public:
        vec3 pos;
        vec3 vel;
        float mass;
        bool fixed;

        Atom(vec3 p_pos, float p_mass, bool p_fixed) : pos(p_pos), mass(p_mass), fixed(p_fixed), vel(vec3(0, 0, 0)) {
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

        void dampen(float delta_t) {
            vel.x *= 1.0f - damping * delta_t;
            vel.y *= 1.0f - damping * delta_t;
            vel.z *= 1.0f - damping * delta_t;
        }

        float velocity_length() {
            return sqrtf(vel.x*vel.x + vel.y*vel.y + vel.z*vel.z);
        }

        float velocity_sqr() {
            return vel.x*vel.x + vel.y*vel.y + vel.z*vel.z;
        }

        void collide_with_floor() {
            if(pos.z <= 0) {
                vel.z = abs(vel.z);
            }
        }
};
