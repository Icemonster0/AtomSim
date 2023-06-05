#pragma once

#include "atom.h"
#include <cmath>

class Spring {
    public:
        Atom* atom_a;
        Atom* atom_b;
        float resting_length;

        Spring(Atom* p_atom_a, Atom* p_atom_b, float p_resting_length) :
            atom_a(p_atom_a), atom_b(p_atom_b), resting_length(p_resting_length) {
        }

        float calculate_length() {
            // return std::hypotf(std::hypotf(atom_a->pos.x - atom_b->pos.x, atom_a->pos.y - atom_b->pos.y), atom_a->pos.z - atom_b->pos.z);
            float xdiff = atom_a->pos.x - atom_b->pos.x;
            float ydiff = atom_a->pos.y - atom_b->pos.y;
            float zdiff = atom_a->pos.z - atom_b->pos.z;
            // std::cout << xdiff << " " << ydiff << " " << zdiff << " " << std::endl;
            // std::cout << std::endl << "atom_a->pos.x " << atom_a->pos.x << std::endl;
            float d = sqrtf(xdiff*xdiff + ydiff*ydiff);
            float l = sqrtf(d*d + zdiff*zdiff);
            return l;
        }
};
