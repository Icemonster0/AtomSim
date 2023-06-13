#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <time.h>

// Util
#include "util.h"

// Global Settings
#include "settings.h"

// Classes
#include "tri.h"
#include "spring.h"
#include "atom.h"
#include "mesh.h"

// Variables
extern Mesh mesh;

// Functions
void start_sim();
void render(int frame_i);
