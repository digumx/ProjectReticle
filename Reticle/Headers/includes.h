#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

#define DEBUG_CODE
#define VERBOSE_ON
//#define PLATFORM_LINUX
//#define PLATFORM_WINDOWS

#define GLEW_NO_GLU
#ifdef PLATFORM_WINDOWS
#define GLEW_STATIC
#endif // PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef PLATFORM_LINUX
#define FILEPATH_SEPARATOR '/'
#endif // PLATFORM_WINDOWS
#ifdef PLATFORM_WINDOWS
#define FILEPATH_SEPARATOR '\\'
#endif // PLATFORM_WINDOWS

#define GLM_FORCE_RADIANS
/// WARNING: GLM_FORCE_SWIZZLE causes strange errors. DO NOT USE!!!
//#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION

#endif // INCLUDES_H_INCLUDED
