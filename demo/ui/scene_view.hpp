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

class SceneView {
    public:
        
        void init(int32_t width, int32_t height);
    
        void render();
    
        FrameBuffer frameBuffer;
        VertexBuffer vertexBuffer;
        Shader shader;
        bMath::float2 size;
};

#endif