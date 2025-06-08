#ifndef BWINDOW
#define BWINDOW

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "imgui_helper.hpp"
#include "scene.hpp"
#include "properties.hpp"

class GLWindow {
    public:
        void init(int width, int height, const char* title);

        void render();
    
        GLFWwindow* glfwWindow;

        const char* m_title;
        int m_width;
        int m_height;

        Scene m_scene;
        PropertiesPanel m_properties;
};

#endif