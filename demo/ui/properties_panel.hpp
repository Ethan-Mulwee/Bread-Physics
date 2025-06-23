#ifndef BENGINE_PROPERTIES_PANEL
#define BENGINE_PROPERTIES_PANEL

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "scene_view.hpp"

#include "bMath.hpp"

class PropertiesPanel {
    public:
        void render(SceneView &sceneView);

    bMath::float3 color;
};

#endif