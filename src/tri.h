#pragma once

#include "atom.h"
#include "util.h"


struct tri {
    Atom* a;
    Atom* b;
    Atom* c;
    bool a_tmp; // Only needed for sphere
    bool b_tmp; // meshing, doesn't affect
    bool c_tmp; // anything else.

    tri(Atom* pa, Atom* pb, Atom* pc) : a(pa), b(pb), c(pc), a_tmp(true), b_tmp(true), c_tmp(true) {
    }
};
