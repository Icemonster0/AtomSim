#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm>

// Util
#include "util.h"

// Global Settings
#include "settings.h"

// Objects
#include "spring.h"
#include "atom.h"
#include "mesh.h"

// Variables
extern Mesh mesh;

// Functions
void start_sim();
void render(int frame_i);
