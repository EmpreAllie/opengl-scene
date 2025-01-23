#ifndef CALLBACKS_H_DEFINED
#define CALLBACKS_H_DEFINED

#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CScene.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void CursorPosCallback(GLFWwindow* window, double xPos, double yPos);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data);

#endif