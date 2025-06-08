#ifndef BUI
#define BUI

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UI {
    public:

    void intialize(GLFWwindow* window); 

    void intialize_render();
};

#endif