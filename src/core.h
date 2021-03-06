#pragma once

// system libs
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

// c libs
#include "string.h"
#include <stdarg.h>

// c++ libs
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>
#include <algorithm>
#include <functional>

// project libs
#ifdef __linux__
#include <GL/glx.h>
#include <GL/glext.h>
#endif

#include <GLFW/glfw3.h>

#ifdef __APPLE__
#undef __gl_h_
#include <OpenGL/gl3.h>
#endif


// #include <GL/glew.h>
#include <linmath.h>
#include "interfaces.hpp"

#define SEEN_RAND_F_DENOMINATOR 2048

#define SEEN_TERM_GREEN "\033[0;32m"
#define SEEN_TERM_RED "\033[1;31m"
#define SEEN_TERM_COLOR_OFF "\033[0m"

namespace seen {

extern std::string DATA_PATH;

bool gl_get_error();

float rf();
float rf(float min, float max);
Vec3 rn();

}
