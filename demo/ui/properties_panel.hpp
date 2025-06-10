#ifndef BENGINE_PROPERTIES_PANEL
#define BENGINE_PROPERTIES_PANEL

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"

class PropertiesPanel {
    public:
        void render();

    bMath::float3 color;
};

#endif