#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>

// Util
#include "util.h"

// Global Settings
#include "settings.h"

// Objects
#include "spring.h"
#include "atom.h"

// Variables
extern std::vector<Atom> atom_list;
extern std::vector<Spring> spring_list;

// Functions
void start_sim();
void render(int frame_i);
