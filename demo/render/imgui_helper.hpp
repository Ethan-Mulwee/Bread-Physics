#ifndef BUI
#define BUI

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiHelper {
    public:

    static void init(GLFWwindow* window); 

    static void intialize_render();

    static void render();
};

#endif