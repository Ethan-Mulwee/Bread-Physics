#ifndef BENGINE_IMGUIHELPER
#define BENGINE_IMGUIHELPER

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiHelperFunctions {
    public:

    static void init(GLFWwindow* window); 

    static void intializeRender();

    static void render();
};

#endif