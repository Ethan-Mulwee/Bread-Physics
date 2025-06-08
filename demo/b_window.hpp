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
    
        GLFWwindow* m_GLFWWindow;

        const char* m_Title;
        int m_Width;
        int m_Height;

        SceneRenderer m_SceneRenderer;
        PropertiesPanel m_PropertiesPanel;
};

#endif