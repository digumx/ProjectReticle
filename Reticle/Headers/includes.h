#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

#define DEBUG_CODE
#define VERBOSE_ON
//#define PLATFORM_WINDOWS
//#define PLATFORM_LINUX

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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION

#endif // INCLUDES_H_INCLUDED
