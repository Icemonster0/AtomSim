#pragma once

#include "util.h"

class Atom {
    public:
        vec3 pos;
        vec3 vel;
        float mass;

        Atom(vec3 p_pos, float p_mass) : pos(p_pos), vel(vec3(0, 0, 0)), mass(p_mass) {
        }
};
