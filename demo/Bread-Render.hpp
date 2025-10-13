#ifndef DEMO_WINDOW
#define DEMO_WINDOW

#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "smath.hpp"

struct GLWindow {
    GLFWwindow* glfwWindow;
    int width, height;
    const char* name;
    
    smath::vector2 mousePos = {0.0f,0.0f};
    smath::vector2 deltaMousePos = {0.0f,0.0f};
    float scrollInput = 0.0f;
    float time = 0.0f;
    float deltaTime = 0.0f;
    double perviousRenderTime = 0.0;
    double perviousPhysicsTime = 0.0;
};

GLWindow *createWindow(int width, int height, const char *name);

void updateWindow(GLWindow *window);

void destroyWindow(GLWindow *window);

#endif