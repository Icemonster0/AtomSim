#pragma once

#include <string>


struct vec3 {
    float x, y, z;

    vec3(float px, float py, float pz) : x(px), y(py), z(pz) {
    }
};


struct color {
    float r, g, b;

    color() : r(0), g(0), b(0) {
    }

    color(float pr, float pg, float pb) : r(pr), g(pg), b(pb) {
    }
};
