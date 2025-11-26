#pragma once

#if BUILD_PLATFORM_WINDOWS
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#elif BUILD_PLATFORM_APPLE
#include <GLUT/glut.h>
#elif BUILD_PLATFORM_LINUX
#include "GL/freeglut.h"
#endif