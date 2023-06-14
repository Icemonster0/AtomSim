#pragma once

#include "atom.h"
#include "util.h"


struct tri {
    Atom* a;
    Atom* b;
    Atom* c;
    // bool a_tmp; // Only needed for sphere
    // bool b_tmp; // meshing, doesn't affect
    // bool c_tmp; // anything else.
    int ai;
    int bi;
    int ci;

    color col;

    tri(Atom* pa, Atom* pb, Atom* pc) : a(pa), b(pb), c(pc), col(color(1.0, 1.0, 1.0)) {
    }

    tri(int pa, int pb, int pc) : ai(pa), bi(pb), ci(pc), col(color(1.0, 1.0, 1.0)) {
    }

    void fill_pointers(Atom** vec) {
        a = vec[ai];
        b = vec[bi];
        c = vec[ci];
    }
};
