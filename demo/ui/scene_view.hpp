#ifndef BSCENE
#define BSCENE

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "bMath.hpp"

#include "render/framebuffer.hpp"
#include "render/vertexbuffer.hpp"
#include "shader/b_shader.hpp"
#include "objects/camera.hpp"

class SceneView {
    public:
        
        void init(int32_t width, int32_t height);
    
        void render();

        void onScroll(double delta);

        FrameBuffer frameBuffer;
        VertexBuffer vertexBuffer;
        Shader m_Shader;
        Camera m_Camera;

        bMath::float2 m_Size;
};

#endif