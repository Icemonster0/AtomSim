#pragma once

#include <string>

struct vec3 {
    float x, y, z;

    vec3() : x(0), y(0), z(0) {
    }

    vec3(float px, float py, float pz) : x(px), y(py), z(pz) {
    }

    inline vec3 operator-(vec3 v) {
        return {x - v.x, y - v.y, z - v.z};
    }

    inline vec3 operator+(vec3 v) {
        return {x + v.x, y + v.y, z + v.z};
    }

    inline vec3 operator*(float f) {
        return {x * f, y * f, z * f};
    }

    inline vec3 operator/(float f) {
        return {x / f, y / f, z / f};
    }
};


struct color {
    float r, g, b;

    color() : r(0), g(0), b(0) {
    }

    color(float pr, float pg, float pb) : r(pr), g(pg), b(pb) {
    }
};
