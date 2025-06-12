#ifndef BENGINE_WINDOW
#define BENGINE_WINDOW

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "render/imgui_helper.hpp"
#include "ui/scene_view.hpp"
#include "ui/properties_panel.hpp"

class GLWindow {
    public:
        void init(int width, int height, const char* title);

        void render();

        static void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
        GLFWwindow* m_GLFWWindow;

        const char* m_Title;
        int m_Width;
        int m_Height;

        static SceneView s_SceneView;
        PropertiesPanel m_PropertiesPanel;
};


#endif