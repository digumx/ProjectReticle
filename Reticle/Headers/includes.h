#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

#define DEBUG_CODE
#define VERBOSE_ON
#define PLATFORM_LINUX
//#define PLATFORM_WINDOWS

#define GLEW_NO_GLU
#ifdef PLATFORM_WINDOWS
#define GLEW_STATIC
#endif // PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION

#endif // INCLUDES_H_INCLUDED
